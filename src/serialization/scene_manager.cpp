#include "scene_manager.h"

#include "scene_serializer.h"
#include "src/serialization/serializers/gltf_core_node_serializer.h"
#include "src/serialization/serializers/buffer_serializer.h"
#include "src/serialization/serializers/mesh_serializer.h"
#include "src/serialization/serializers/image_serializer.h"
#include "src/serialization/serializers/material_serializer.h"
#include "src/serialization/serializers/sampler_serializer.h"
#include "src/serialization/serializers/texture_serializer.h"
#include "src/flecs_modules/freefly_camera/freefly.h"
#include "src/editor/editor.h"

scene_manager::scene_manager(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames) {
    for(const auto& scene : scene_filenames) {
        scenes[scene.first] = scene.second;
    }

}

int scene_manager::get_current_scene_index() const {
    return loaded_scene;
}

void scene_manager::unload_scene() {
    main_world.reset();
}

void scene_manager::update() {
    if(main_world.has<scene_swap_command>()) {
        const auto* swap_command = main_world.get<scene_swap_command>();
        if(std::holds_alternative<unsigned int>(swap_command->scene)) {
            load_scene(std::get<unsigned int>(swap_command->scene), swap_command->load_as_editor);
        }
        else if(std::holds_alternative<std::string>(swap_command->scene)) {
            load_scene(std::get<std::string>(swap_command->scene), swap_command->load_as_editor);
        }
    }
    if(main_world.has<scene_save_command>()) {
        const auto* save_command = main_world.get<scene_save_command>();
        if(save_command->save_path) {
            serializer.save_scene_to_file(main_world, save_command->save_path.value());
        }
        else {
            serializer.save_scene_to_file(main_world, scenes[loaded_scene]);
        }
        main_world.remove<scene_save_command>();
    }
    if(main_world.has<new_scene_command>()) {
        load_scene("default.gltf", true);
    }
    if(main_world.has<switch_play_command>()) {
        const auto* switch_command = main_world.get<switch_play_command>();

        if(switch_command->editor && !in_editor) {
            set_editor();
        }
        else if(!switch_command->editor && in_editor) {
            set_play();
        }

        main_world.remove<switch_play_command>();
    }
}

void scene_manager::load_scene(unsigned int scene_index, bool editor) {
    load_scene(scenes[scene_index], editor);
}

void scene_manager::load_scene(std::string scene_filename, bool editor) {
    unload_scene();
    editor_only_onstart = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::OnStart);
    editor_only_preupdate = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::PreUpdate);
    editor_only_onupdate = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::OnUpdate);
    editor_only_onstore = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::OnStore);
    play_and_editor_onstart = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::OnStart);
    play_and_editor_preupdate = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::PreUpdate);
    play_and_editor_onupdate = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::OnUpdate);
    play_and_editor_onstore = main_world.entity()
        .add(flecs::Phase)
        .depends_on(flecs::OnStore);

    if(editor) {
        set_editor();
    }
    else {
        set_play();
    }

    main_world.import<transformation>();
    main_world.import<rendering>();
    main_world.import<freefly>();

    serializer.load_scene_from_file(main_world, scene_filename);
}

void scene_manager::set_play() {
    play_pipeline = main_world.pipeline()
            .with(flecs::System)
            .with(flecs::Phase).cascade(flecs::DependsOn)
            .without(flecs::Disabled).up(flecs::DependsOn)
            .without(flecs::Disabled).up(flecs::ChildOf)
            .without(editor_only_onstart)
            .without(editor_only_onstore)
            .without(editor_only_onupdate)
            .build();
    in_editor = false;
    main_world.set_pipeline(play_pipeline);
    render_debug_camera debug {};
    debug.debug_camera = {};
    main_world.set<render_debug_camera>(debug);
}

void scene_manager::set_editor() {
    editor_pipeline = main_world.pipeline()
            .with(flecs::System)
            .with(editor_only_preupdate).or_()
            .with(editor_only_onupdate).or_()
            .with(editor_only_onstore).or_()
            .with(play_and_editor_onupdate).or_()
            .with(play_and_editor_onstore).or_()
            .with(play_and_editor_preupdate)
            .without(flecs::Disabled).up(flecs::DependsOn)
            .without(flecs::Disabled).up(flecs::ChildOf)
            .build();
    in_editor = true;
    main_world.set_pipeline(editor_pipeline);
    render_debug_camera debug {};
    flecs::entity editor_camera = main_world.entity()
            .set<position>({})
            .set<rotation>({})
            .set<scale>({})
            .set<freefly_controller>({})
            .set<camera>({camera::projection::perspective});
    debug.debug_camera = editor_camera;
    main_world.set<render_debug_camera>(debug);
}



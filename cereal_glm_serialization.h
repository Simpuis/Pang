#pragma once

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>

namespace glm
{
	template<class Archive, class T>
	void serialize(Archive& archive, glm::vec<2, T, glm::defaultp> &v)
	{
		archive(cereal::make_nvp("x", v.x), 
				cereal::make_nvp("y", v.y));
	}

	template<class Archive, class T>
	void serialize(Archive& archive, glm::vec<3, T, glm::defaultp> &v)
	{
		archive(cereal::make_nvp("x", v.x), 
				cereal::make_nvp("y", v.y), 
				cereal::make_nvp("z", v.z));
	}

	template<class Archive, class T>
	void serialize(Archive& archive, glm::vec<4, T, glm::defaultp> &v)
	{
		archive(cereal::make_nvp("x", v.x), 
				cereal::make_nvp("y", v.y), 
				cereal::make_nvp("z", v.z), 
				cereal::make_nvp("w", v.w));
	}

	template<class Archive, class T>
	void serialize(Archive& archive, glm::mat<2, 2, T, glm::defaultp> &m)
	{
		archive(m[0], m[1]);
	}

	template<class Archive, class T>
	void serialize(Archive& archive, glm::mat<3, 3, T, glm::defaultp> &m)
	{
		archive(m[0], m[1], m[2]);
	}

	template<class Archive, class T>
	void serialize(Archive& archive, glm::mat<4, 4, T, glm::defaultp> &m)
	{
		archive(m[0], m[1], m[2], m[3]);
	}

}

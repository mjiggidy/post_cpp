#pragma once
#include <string>
#include <unordered_map>
#include "timecode.h"

namespace post {
	class Shot
	{
		const std::string m_shot_name;
		const Timecode m_tc_start;
		const Timecode m_tc_duration;
		const float m_framerate;
		std::unordered_map<std::string, std::string> m_metadata;

		void Validate() const;

	public:
		Shot(const std::string& name, const Timecode& tc_start, const Timecode& tc_duration, const float& framerate);
		Shot(const std::string& name, const std::string& tc_start, const std::string& tc_duration, const float& framerate, bool is_dropframe = false);
		Shot(const std::string& name, const long& tc_start, const long& tc_duration, const float& framerate, bool is_dropframe = false);

		std::string GetShotName() const;

		Timecode GetTimecodeStart() const;
		long GetFrameStart() const;

		Timecode GetTimecodeEnd() const;
		long GetFrameEnd() const;

		Timecode GetTimecodeDuration() const;
		long GetFrameCount() const;

		float GetFrameRate() const;

		void AddMetadata(const std::string& key, const std::string& value);

		bool HasMetadata(const std::string& key) const;

		std::string GetMetadata(const std::string& key) const;
		std::unordered_map<std::string, std::string> GetMetadata() const;

		bool operator== (const Shot& comp) const;
		bool operator< (const Shot& comp) const;
	};
}
#include <string>
#include <unordered_map>
#include "shot.h"

namespace post {


		void Shot::Validate() const
		{
			// Verify timecode rates are compatible with the video frame rate
			if (round(m_framerate) != m_tc_start.GetRate() || round(m_framerate) != m_tc_duration.GetRate())
				throw std::exception("Video frame rate is incompatible with timecode rate");

			// Verify start timecode is not negative
			if (m_tc_start.IsNegative())
				throw std::exception("Start timecode must not be negative");

			// Verify timecode duration is at least one frame long
			if (m_tc_duration.GetFrameNumber() < 1)
				throw std::exception("Shot must be at least one frame long");
		}

		// Construct shot from Timecode objects
		Shot::Shot(const std::string& name, const Timecode& tc_start, const Timecode& tc_duration, const float& framerate)
			: m_shot_name(name), m_tc_start(tc_start), m_tc_duration(tc_duration), m_framerate(framerate)
		{
			Validate();
		}

		// Construct shot from timecode strings
		Shot::Shot(const std::string& name, const std::string& tc_start, const std::string& tc_duration, const float& framerate, bool is_dropframe)
			: m_shot_name(name), m_tc_start(Timecode(tc_start, round(framerate), is_dropframe)), m_tc_duration(Timecode(tc_duration, round(framerate), is_dropframe)), m_framerate(framerate)
		{
			Validate();
		}

		// Construct shot from timecode values
		Shot::Shot(const std::string& name, const long& tc_start, const long& tc_duration, const float& framerate, bool is_dropframe)
			: m_shot_name(name), m_tc_start(Timecode(tc_start, round(framerate), is_dropframe)), m_tc_duration(Timecode(tc_duration, round(framerate), is_dropframe)), m_framerate(framerate)
		{
			Validate();
		}

		std::string Shot::GetShotName() const
		{
			return m_shot_name;
		}

		Timecode Shot::GetTimecodeStart() const
		{
			return m_tc_start;
		}
		long Shot::GetFrameStart() const
		{
			return GetTimecodeStart().GetFrameNumber();
		}

		Timecode Shot::GetTimecodeEnd() const
		{
			return m_tc_start + m_tc_duration;
		}
		long Shot::GetFrameEnd() const
		{
			return GetTimecodeEnd().GetFrameNumber();
		}

		Timecode Shot::GetTimecodeDuration() const
		{
			return m_tc_duration;
		}
		long Shot::GetFrameCount() const
		{
			return GetTimecodeDuration().GetFrameNumber();
		}

		float Shot::GetFrameRate() const
		{
			return m_framerate;
		}

		void Shot::AddMetadata(const std::string& key, const std::string& value)
		{
			if (key.length() > 0 && value.length() > 0)
				m_metadata[key] = value;
		}

		bool Shot::HasMetadata(const std::string& key) const
		{
			return m_metadata.count(key) > 0;
		}

		std::string Shot::GetMetadata(const std::string& key) const
		{
			if (HasMetadata(key))
				return m_metadata.at(key);
			else
				return "";
		}
		std::unordered_map<std::string, std::string> Shot::GetMetadata() const
		{
			return m_metadata;
		}

		bool Shot::operator== (const Shot& comp) const
		{
			return (comp.GetShotName() == GetShotName() && comp.GetFrameRate() == GetFrameRate() && comp.GetTimecodeStart() == GetTimecodeStart() && comp.GetTimecodeDuration() == GetTimecodeDuration());
		}
		bool Shot::operator< (const Shot& comp) const
		{
			if (GetShotName() == comp.GetShotName())
			{
				if (GetFrameRate() == comp.GetFrameRate())
					return GetTimecodeStart() < comp.GetTimecodeStart();
				else
					return GetFrameRate() < comp.GetFrameRate();
			}

			else
				return GetShotName() < comp.GetShotName();
		}
}

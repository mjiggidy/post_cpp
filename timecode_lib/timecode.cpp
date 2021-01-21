#include <iostream>
#include <string>
#include <regex>
#include <exception>
#include <deque>
#include "timecode.h"

namespace post {

	bool Timecode::IsCompatible(const Timecode& comp) const
	{
		return (comp.GetRate() == GetRate()) && (comp.IsDropFrame() == IsDropFrame());
	}

	int Timecode::DropFrameOffset() const
	{
		// Two frames of timecode are dropped every minute, except on the tenth minute, when nothing Is dropped

		return 0;

	}

	Timecode::Timecode(const long& fc, const unsigned short& fr, bool dropframe) : m_frame_number(fc), m_frame_rate(fr), m_drop_frame(dropframe)
	{
		Validate();
	}

	Timecode::Timecode(const std::string& tc, const unsigned short& fr, bool dropframe) : m_frame_number(0), m_frame_rate(round(fr)), m_drop_frame(dropframe)
	{

		// Validate
		std::regex re(R"([+\-]?\d+(?:[:;](\d+)){0,3})");
		std::smatch m;

		if (!std::regex_match(tc, m, re))
		{
			throw std::exception("Invalid timecode provided");
		}

		bool is_neg = false;
		std::deque<int> nums;
		size_t seg_start = 0;

		if (tc[seg_start] == '+' || tc[seg_start] == '-')
		{
			is_neg = tc[seg_start] == '-';
			++seg_start;
		}

		if (tc[seg_start] == ':')
		{
			++seg_start;
		}

		size_t seg_next = 0;
		while ((seg_next = tc.find(":", seg_start)) != std::string::npos)
		{
			nums.push_front(std::stoi(tc.substr(seg_start, seg_next - seg_start)));
			seg_start = seg_next + 1;
		}
		nums.push_front(std::stoi(tc.substr(seg_start)));
		nums.resize(4);


		m_frame_number += nums[0];
		m_frame_number += nums[1] * m_frame_rate;
		m_frame_number += nums[2] * m_frame_rate * 60;
		m_frame_number += nums[3] * m_frame_rate * 60 * 60;

		if (is_neg)
		{
			m_frame_number *= -1;
		}

		Validate();
	}

	bool Timecode::Validate() const
	{
		if (m_frame_rate <= 0)
			throw std::exception("Frame rate must be a positive non-zero value.");

		if (m_drop_frame && (m_frame_rate % 30))
			throw std::exception("Drop frame Is only supported for multiples of 30fps.");

		return true;
	}

	short Timecode::GetRate() const
	{
		return m_frame_rate;
	}

	long Timecode::GetFrameNumber() const
	{
		return m_frame_number;
	}

	short Timecode::GetFrames() const
	{
		return (m_frame_number + DropFrameOffset()) % m_frame_rate;
	}

	short Timecode::GetSeconds() const
	{
		return (m_frame_number + DropFrameOffset()) / m_frame_rate % 60;
	}

	short Timecode::GetMinutes() const
	{
		return (m_frame_number + DropFrameOffset()) / m_frame_rate / 60 % 60;
	}

	short Timecode::GetHours() const
	{
		return (m_frame_number + DropFrameOffset()) / m_frame_rate / 60 / 60;
	}

	bool Timecode::IsDropFrame() const
	{
		return m_drop_frame;
	}

	bool Timecode::IsNegative() const
	{
		return (m_frame_number < 0);
	}

	bool Timecode::operator== (const Timecode& comp) const
	{
		return IsCompatible(comp) && (comp.GetFrameNumber() == GetFrameNumber());
	}

	bool Timecode::operator!= (const Timecode& comp) const
	{
		return !(*this == comp);	// I mean... right...?
	}

	bool Timecode::operator< (const Timecode& comp) const
	{
		if (IsCompatible(comp))
			return GetFrameNumber() < comp.GetFrameNumber();
		else
			return GetRate() < comp.GetRate();
	}

	bool Timecode::operator> (const Timecode& comp) const
	{
		if (IsCompatible(comp))
			return GetFrameNumber() > comp.GetFrameNumber();
		else
			return GetRate() > comp.GetRate();
	}

	Timecode Timecode::operator+(const Timecode& opr) const
	{
		if (!IsCompatible(opr))
			throw std::exception("Timecodes must be the same rate");

		return Timecode(GetFrameNumber() + opr.GetFrameNumber(), GetRate(), IsDropFrame());
	}

	Timecode Timecode::operator-(const Timecode& opr) const
	{
		if (!IsCompatible(opr))
			throw std::exception("Timecodes must be the same rate");

		return Timecode(GetFrameNumber() - opr.GetFrameNumber(), GetRate(), IsDropFrame());
	}

	Timecode& Timecode::operator++()
	{
		++m_frame_number;
		return *this;
	}

	Timecode& Timecode::operator--()
	{
		--m_frame_number;
		return *this;
	}

	std::string Timecode::GetTimecode() const
	{

		std::string output;
		unsigned short int hours = abs(GetHours());
		unsigned short int minutes = abs(GetMinutes());
		unsigned short int seconds = abs(GetSeconds());
		unsigned short int frames = abs(GetFrames());

		if (m_frame_number < 0)
			output += "-";

		if (hours < 10) output += "0";
		output += std::to_string(hours);

		output += ":";

		if (minutes < 10) output += "0";
		output += std::to_string(minutes);
		output += ":";

		if (seconds < 10) output += "0";
		output += std::to_string(seconds);
		output += (m_drop_frame ? ";" : ":");

		if (frames < 10) output += "0";
		output += std::to_string(frames);

		return output;
	}

	std::ostream& operator<< (std::ostream& stream, const Timecode& tc)
	{
		stream << tc.GetTimecode();
		return stream;
	}
}
#pragma once

namespace post
{
	class Timecode
	{
	private:
		long  m_frame_number;	// Not const? Can increment
		const short m_frame_rate;
		const bool  m_drop_frame;

		bool Validate() const;
		bool IsCompatible(const Timecode&) const;
		int  DropFrameOffset() const;

	public:
		Timecode(const long& fc, const unsigned short& = 24, bool = false);
		Timecode(const std::string&, const unsigned short& = 24, bool = false);

		long  GetFrameNumber() const;
		short GetRate()        const;
		bool  IsDropFrame()    const;
		bool  IsNegative()     const;

		short GetFrames()  const;
		short GetSeconds() const;
		short GetMinutes() const;
		short GetHours()   const;

		bool operator== (const Timecode&) const;
		bool operator!= (const Timecode&) const;
		bool operator<  (const Timecode&) const;
		bool operator>  (const Timecode&) const;

		Timecode operator+ (const Timecode&) const;
		Timecode operator- (const Timecode&) const;

		Timecode& operator++ ();
		Timecode& operator-- ();

		friend std::ostream& operator<< (std::ostream&, const Timecode&);

		std::string GetTimecode() const;
	};
}
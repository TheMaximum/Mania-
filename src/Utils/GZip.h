#ifndef GZIP_H_
#define GZIP_H_

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

//* GZip
/**
 * \brief Utility to gzip strings.
 */
class GZip {
public:
	/*!
	 * \brief Compresses provided data.
	 *
	 * \param data Data to be compressed.
	 */
	static std::string Compress(const std::string& data)
	{
		namespace bio = boost::iostreams;

		std::stringstream compressed;
		std::stringstream origin(data);

		bio::filtering_streambuf<bio::input> out;
		out.push(bio::gzip_compressor(bio::gzip_params(bio::gzip::best_compression)));
		out.push(origin);
		bio::copy(out, compressed);

		return compressed.str();
	}

	/*!
	 * \brief Decompresses provided data.
	 *
	 * \param data Data to be decompressed.
	 */
	static std::string Decompress(const std::string& data)
	{
		namespace bio = boost::iostreams;

		std::stringstream compressed(data);
		std::stringstream decompressed;

		bio::filtering_streambuf<bio::input> out;
		out.push(bio::gzip_decompressor());
		out.push(compressed);
		bio::copy(out, decompressed);

		return decompressed.str();
	}
};

#endif // GZIP_H_

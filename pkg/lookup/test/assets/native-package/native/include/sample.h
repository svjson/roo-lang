#ifndef SAMPLE_NATIVE_H
#define SAMPLE_NATIVE_H

namespace Sample
{
  /*!
   * @brief Read fixture data from a path.
   *
   * The fixture keeps this as a separate paragraph.
   *
   * Usage:
   * @code
   * (sample.native/read! "fixture.txt")
   * => "contents"
   * @endcode
   *
   * | Arg # | Description |
   * |-------|-------------|
   * | 0     | Path to read. |
   */
  FUNC(SampleReadFunction, read)
} // namespace Sample

#endif

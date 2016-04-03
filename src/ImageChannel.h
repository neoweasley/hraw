/*
 *  HRAW - Hacker's toolkit for image sensor characterisation
 *  Copyright 2016 Ciriaco Garcia de Celis
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGECHANNEL_H_
#define IMAGECHANNEL_H_

#include "ImageSelection.h"

struct FilterPattern // allows to represent any (simple) periodic pixel pattern
{
    imgsize_t xshift; // 0  1  0  1  <-- example for a RGGB matrix:
    imgsize_t yshift; // 0  0  1  1                R  G1
    imgsize_t xdelta; // 2  2  2  2                G2  B
    imgsize_t ydelta; // 2  2  2  2
                      // R G1 G2  B

    static FilterPattern RGGB_R()  { return FilterPattern { 0, 0, 2, 2 };  }
    static FilterPattern RGGB_G1() { return FilterPattern { 1, 0, 2, 2 };  }
    static FilterPattern RGGB_G2() { return FilterPattern { 0, 1, 2, 2 };  }
    static FilterPattern RGGB_B()  { return FilterPattern { 1, 1, 2, 2 };  }
    static FilterPattern FULL()    { return FilterPattern { 0, 0, 1, 1 };  } // full plain image (all pixels)
};

class ImageChannel : public std::enable_shared_from_this<ImageChannel> // virtualizes a color channel selection
{
        ImageChannel& operator=(const ImageChannel&) = delete;
        ImageChannel(const ImageChannel&) = delete;

  public:

        typedef std::shared_ptr<const ImageChannel> ptr;

        explicit ImageChannel(const std::shared_ptr<const class RawImage>& rawImage, const FilterPattern& filterPattern)
          : raw(rawImage),
            pattern(filterPattern)
        {}

        const std::shared_ptr<const class RawImage> raw;
        const FilterPattern pattern;

        imgsize_t width() const;
        imgsize_t height() const;

        ImageSelection::ptr select() const // full channel
        {
            return ImageSelection::ptr(new ImageSelection(shared_from_this(), 0, 0, width(), height()));
        }

        ImageSelection::ptr select(imgsize_t cx, imgsize_t cy, imgsize_t selectedWidth, imgsize_t selectedHeight) const
        {
            select();
            return ImageSelection::ptr(new ImageSelection(shared_from_this(), cx, cy, selectedWidth, selectedHeight));
        }

        ImageSelection::ptr select(double partsPerUnit) const; // square from center
};

#endif /* IMAGECHANNEL_H_ */
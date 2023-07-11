//
// Created by Tymoteusz Pilarz on 10/06/2023.
//

#ifndef IMAGECOLORIZER_CONTENT_H
#define IMAGECOLORIZER_CONTENT_H

#include <tuple>
#include <vector>
#include <string>

#include <Wt/WContainerWidget.h>
#include <Wt/WColor.h>

#include "ImagePainter.h"

class Content : public Wt::WContainerWidget
{
private:
    int width = 0;
    int height = 0;

    ImagePainter* imagePainter;

    double gamma = 2.0;
    int threshold = 10;

    static std::tuple<int, int> getScaledSize(int sourceWidth, int sourceHeight, int destinationWidth, int destinationHeight);

protected:
    void layoutSizeChanged(int width, int height) override;

public:
    Content();

    void setImage(const std::string& fileName);

    void setPenColor(const Wt::WColor& color);
    void setPenWidth(int width);
    [[nodiscard]] Wt::WColor getPenColor() const;
    [[nodiscard]] int getPenWidth() const;

    void undo();
    void redo();
    void clearCanvas();

    void setGamma(double gamma);
    void setThreshold(int threshold);
    [[nodiscard]] double getGamma() const;
    [[nodiscard]] int getThreshold() const;

    void colorize(const std::string& outputFileName);
    void hideResult();
};

#endif //IMAGECOLORIZER_CONTENT_H

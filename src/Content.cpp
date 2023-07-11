//
// Created by Tymoteusz Pilarz on 10/06/2023.
//

#include <memory>
#include <filesystem>

#include <opencv2/highgui.hpp>

#include "Content.h"
#include "Colorize.h"

using namespace Wt;

std::tuple<int, int> Content::getScaledSize(int sourceWidth, int sourceHeight, int destinationWidth, int destinationHeight)
{
    auto widthRatio = static_cast<double>(destinationWidth) / sourceWidth;
    auto heightRatio = static_cast<double>(destinationHeight) / sourceHeight;

    auto scaleRatio = (widthRatio < heightRatio) ? widthRatio : heightRatio;

    auto width = static_cast<int>(sourceWidth * scaleRatio);
    auto height = static_cast<int>(sourceHeight * scaleRatio);

    return {width, height};
}

void Content::layoutSizeChanged(int width, int height)
{
    this->width = width;
    this->height = height;

    auto [imagePainterWidth, imagePainterHeight] = getScaledSize(imagePainter->getImageWidth(), imagePainter->getImageHeight(), width, height);

    imagePainter->resize(imagePainterWidth, imagePainterHeight);

    WWidget::layoutSizeChanged(width, height);
}

Content::Content()
{
    setStyleClass("content");
    setOverflow(Overflow::Visible);
    setLayoutSizeAware(true);
    setContentAlignment(AlignmentFlag::Center);
    WWebWidget::setMargin(0);

    imagePainter = addWidget(std::make_unique<ImagePainter>());
}

void Content::setImage(const std::string& fileName)
{
    auto image = std::make_unique<WPainter::Image>(fileName, fileName);

    auto [imagePainterWidth, imagePainterHeight] = getScaledSize(image->width(), image->height(), width, height);
    imagePainter->resize(imagePainterWidth, imagePainterHeight);

    imagePainter->setImage(std::move(image));
}

void Content::setPenColor(const WColor& color)
{
    imagePainter->setPenColor(color);
}

void Content::setPenWidth(int width)
{
    imagePainter->setPenWidth(width);
}

WColor Content::getPenColor() const
{
    return imagePainter->getPenColor();
}

int Content::getPenWidth() const
{
    return imagePainter->getPenWidth();
}

void Content::undo()
{
    imagePainter->undo();
}

void Content::redo()
{
    imagePainter->redo();
}

void Content::clearCanvas()
{
    imagePainter->clearCanvas();
}

void Content::setGamma(double gamma)
{
    this->gamma = gamma;
}

void Content::setThreshold(int threshold)
{
    this->threshold = threshold;
}

double Content::getGamma() const
{
    return gamma;
}

int Content::getThreshold() const
{
    return threshold;
}

void Content::colorize(const std::string& outputFileName)
{
    if (!imagePainter->isImageSet())
    {
        return;
    }

    auto imageFileName = imagePainter->getImageFileName();
    auto scribblesFileName = std::filesystem::path(outputFileName).parent_path().string() + "/scribbles.png";

    imagePainter->saveScribblesToPNG(scribblesFileName);

    try
    {
        auto image = cv::imread(imageFileName);
        auto scribbles = cv::imread(scribblesFileName);
        auto mask = co::getScribbleMask(image, scribbles, threshold);
        auto colorImage = co::colorize(image, scribbles, mask, gamma);

        cv::imwrite(outputFileName, colorImage);
    }
    catch (const std::runtime_error& e)
    {
        log("error") << e.what();
    }

    auto image = std::make_unique<WPainter::Image>(outputFileName, outputFileName);
    imagePainter->showResult(std::move(image));
}

void Content::hideResult()
{
    imagePainter->hideResult();
}

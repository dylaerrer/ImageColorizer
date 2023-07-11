//
// Created by Tymoteusz Pilarz on 24/06/2023.
//

#ifndef IMAGECOLORIZER_IMAGEPAINTER_H
#define IMAGECOLORIZER_IMAGEPAINTER_H

#include <memory>
#include <vector>
#include <string>

#include <Wt/WBrush.h>
#include <Wt/WColor.h>
#include <Wt/WPen.h>
#include <Wt/WPaintDevice.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPainter.h>
#include <Wt/WPainterPath.h>

enum class Action {update, repaint, result};

class ImagePainter : public Wt::WPaintedWidget
{
private:
    struct BufferElement
    {
        Wt::WPen pen;
        Wt::WPainterPath painterPath;
        int currentWidth;
        int currentHeight;
    };
    std::vector<BufferElement> buffer;
    std::vector<BufferElement> redoBuffer;

    std::unique_ptr<Wt::WPainter::Image> image;
    std::unique_ptr<Wt::WPainter::Image> resultImage;

    Wt::WPainterPath painterPath;
    Wt::WPen pen;

    Action action = Action::update;

    void mouseDown(const Wt::WMouseEvent& e);
    void mouseDrag(const Wt::WMouseEvent& e);

protected:
    void paintEvent(Wt::WPaintDevice* paintDevice) override;

public:
    ImagePainter();

    void resize(const Wt::WLength& width, const Wt::WLength& height) override;

    void setImage(std::unique_ptr<Wt::WPainter::Image> image);
    bool isImageSet() const;
    [[nodiscard]] int getImageWidth() const;
    [[nodiscard]] int getImageHeight() const;
    [[nodiscard]] std::string getImageFileName() const;

    void setPenColor(const Wt::WColor& color);
    void setPenWidth(int width);
    [[nodiscard]] Wt::WColor getPenColor() const;
    [[nodiscard]] int getPenWidth() const;

    void undo();
    void redo();
    void clearCanvas();

    void saveScribblesToPNG(const std::string& fileName);

    void showResult(std::unique_ptr<Wt::WPainter::Image> image);
    void hideResult();
};

#endif //IMAGECOLORIZER_IMAGEPAINTER_H

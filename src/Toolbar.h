//
// Created by Tymoteusz Pilarz on 10/06/2023.
//

#ifndef IMAGECOLORIZER_TOOLBAR_H
#define IMAGECOLORIZER_TOOLBAR_H

#include <memory>
#include <string>

#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WColorPicker.h>
#include <Wt/WDialog.h>

#include "Content.h"

class Toolbar : public Wt::WContainerWidget
{
private:
    Content* content;

    Wt::WPushButton* downloadButton;
    Wt::WPushButton* colorizeButton;
    Wt::WPushButton* hideButton;

    std::string sessionId;
    int uploadFileSuffix = 0;
    int downloadFileSuffix = 0;

    [[nodiscard]] std::unique_ptr<Wt::WHBoxLayout> createLayout();

    [[nodiscard]] std::unique_ptr<Wt::WContainerWidget> createFileMenu();
    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createUploadButton();
    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createDownloadButton();

    [[nodiscard]] std::unique_ptr<Wt::WContainerWidget> createEditMenu();
    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createUndoButton();
    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createRedoButton();
    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createClearButton();

    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createOptionsButton();

    [[nodiscard]] std::unique_ptr<Wt::WColorPicker> createColorPicker();

    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createColorizeButton();
    [[nodiscard]] std::unique_ptr<Wt::WPushButton> createHideButton();

    [[nodiscard]] std::unique_ptr<Wt::WDialog> createOptionsDialog();

    std::string generateUploadFileName();
    [[nodiscard]] std::string getUploadFileName() const;

    std::string generateDownloadFileName();
    [[nodiscard]] std::string getDownloadFileName() const;

public:
    explicit Toolbar(Content* content);
    ~Toolbar() override;
};

#endif //IMAGECOLORIZER_TOOLBAR_H

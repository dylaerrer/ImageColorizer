//
// Created by Tymoteusz Pilarz on 10/06/2023.
//

#include <filesystem>

#include <Wt/WPushButton.h>
#include <Wt/WFileUpload.h>
#include <Wt/WFileResource.h>
#include <Wt/WApplication.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WIntValidator.h>
#include <Wt/WDoubleValidator.h>

#include "Toolbar.h"

using namespace Wt;

std::unique_ptr<WHBoxLayout> Toolbar::createLayout()
{
    auto layout = std::make_unique<WHBoxLayout>();
    layout->setContentsMargins(0, 0, 10, 0);
    layout->setSpacing(0);
    return layout;
}

std::unique_ptr<WContainerWidget> Toolbar::createFileMenu()
{
    auto dropdown = std::make_unique<WContainerWidget>();
    dropdown->setStyleClass("dropdown");

    auto dropdownButton = dropdown->addWidget(std::make_unique<WPushButton>("File"));
    dropdownButton->setStyleClass("dropdown-button");

    auto dropdownContent = dropdown->addWidget(std::make_unique<WContainerWidget>());
    dropdownContent->setStyleClass("dropdown-content");
    dropdownContent->addWidget(createUploadButton());
    dropdownContent->addWidget(createDownloadButton());

    return dropdown;
}

std::unique_ptr<WPushButton> Toolbar::createUploadButton()
{
    auto button = std::make_unique<WPushButton>("Upload");
    button->setStyleClass("dropdown-element");

    auto fileUpload = static_cast<WHBoxLayout*>(layout())->addWidget(std::make_unique<WFileUpload>());
    fileUpload->setDisplayWidget(button.get());
    fileUpload->setWidth(0);
    fileUpload->setHeight(0);
    fileUpload->setMultiple(false);
    fileUpload->setFilters("image/png, image/jpeg");
    fileUpload->changed().connect(fileUpload, &WFileUpload::upload);
    fileUpload->uploaded().connect([this, fileUpload]()
    {
        std::string tempFileName = fileUpload->spoolFileName();
        fileUpload->stealSpooledFile();

        std::string fileName = generateUploadFileName();

        std::filesystem::rename(tempFileName, fileName);

        content->setImage(fileName);
        colorizeButton->enable();
        hideButton->disable();
    });
    fileUpload->fileTooLarge().connect([]()
    {
        log("error") << "File too large";
    });

    return button;
}

std::unique_ptr<WPushButton> Toolbar::createDownloadButton()
{
    auto button = std::make_unique<WPushButton>("Download");
    button->setStyleClass("dropdown-element");
    button->disable();

    downloadButton = button.get();

    return button;
}

std::unique_ptr<WContainerWidget> Toolbar::createEditMenu()
{
    auto dropdown = std::make_unique<WContainerWidget>();
    dropdown->setStyleClass("dropdown");

    auto dropdownButton = dropdown->addWidget(std::make_unique<WPushButton>("Edit"));
    dropdownButton->setStyleClass("dropdown-button");

    auto dropdownContent = dropdown->addWidget(std::make_unique<WContainerWidget>());
    dropdownContent->setStyleClass("dropdown-content");
    dropdownContent->addWidget(createUndoButton());
    dropdownContent->addWidget(createRedoButton());
    dropdownContent->addWidget(createClearButton());

    return dropdown;
}

std::unique_ptr<WPushButton> Toolbar::createUndoButton()
{
    auto button = std::make_unique<WPushButton>("Undo");
    button->setStyleClass("dropdown-element");
    button->clicked().connect(content, &Content::undo);

    return button;
}

std::unique_ptr<WPushButton> Toolbar::createRedoButton()
{
    auto button = std::make_unique<WPushButton>("Redo");
    button->setStyleClass("dropdown-element");
    button->clicked().connect(content, &Content::redo);

    return button;
}

std::unique_ptr<WPushButton> Toolbar::createClearButton()
{
    auto button = std::make_unique<WPushButton>("Clear");
    button->setStyleClass("dropdown-element");
    button->clicked().connect(content, &Content::clearCanvas);

    return button;
}

std::unique_ptr<WPushButton> Toolbar::createOptionsButton()
{
    auto button = std::make_unique<WPushButton>("Options");
    button->setStyleClass("options-button");
    button->clicked().connect([this]()
    {
        auto dialog = this->addChild(createOptionsDialog());
        dialog->show();
        dialog->finished().connect([this, dialog]()
        {
            this->removeChild(dialog);
        });
    });

    return button;
}

std::unique_ptr<WColorPicker> Toolbar::createColorPicker()
{
    auto colorPicker = std::make_unique<WColorPicker>(content->getPenColor());
    colorPicker->setStyleClass("color-picker");
    colorPicker->colorInput().connect([content = content, colorPicker = colorPicker.get()]()
    {
        content->setPenColor(colorPicker->color());
    });

    return colorPicker;
}

std::unique_ptr<WPushButton> Toolbar::createColorizeButton()
{
    auto button = std::make_unique<WPushButton>("Colorize");
    button->setStyleClass("colorize-button");
    button->disable();

    colorizeButton = button.get();

    button->clicked().connect([this]()
    {
        colorizeButton->disable();

        content->colorize(generateDownloadFileName());

        auto imageFile = std::make_shared<WFileResource>("image/png", getDownloadFileName());
        imageFile->suggestFileName("color_image.png");

        WLink link = WLink(imageFile);
        link.setTarget(LinkTarget::NewWindow);

        downloadButton->setLink(link);
        downloadButton->enable();

        hideButton->enable();
    });

    return button;
}

std::unique_ptr<WPushButton> Toolbar::createHideButton()
{
    auto button = std::make_unique<WPushButton>("Hide result");
    button->setStyleClass("colorize-button");
    button->disable();

    hideButton = button.get();

    button->clicked().connect([this]()
    {
        hideButton->disable();
        content->hideResult();
        colorizeButton->enable();
    });

    return button;
}

std::unique_ptr<WDialog> Toolbar::createOptionsDialog()
{
    auto dialog = std::make_unique<WDialog>("Options");
    dialog->setStyleClass("dialog");
    dialog->setModal(false);
    dialog->rejectWhenEscapePressed(true);

    auto widthOptionLayout = dialog->contents()->addWidget(std::make_unique<WContainerWidget>())->setLayout(std::make_unique<WHBoxLayout>());
    widthOptionLayout->addWidget(std::make_unique<WLabel>("Pen width"), 1);
    auto widthOptionInput = widthOptionLayout->addWidget(std::make_unique<WLineEdit>());
    widthOptionInput->setStyleClass("dialog-input");
    auto widthOptionValidator = std::make_shared<WIntValidator>();
    widthOptionValidator->setBottom(1);
    widthOptionValidator->setTop(100);
    widthOptionInput->setValidator(widthOptionValidator);
    widthOptionInput->setMaxLength(3);
    widthOptionInput->setText(std::to_string(content->getPenWidth()));

    auto gammaOptionLayout = dialog->contents()->addWidget(std::make_unique<WContainerWidget>())->setLayout(std::make_unique<WHBoxLayout>());
    gammaOptionLayout->addWidget(std::make_unique<WLabel>("Gamma"), 1);
    auto gammaOptionInput = gammaOptionLayout->addWidget(std::make_unique<WLineEdit>());
    gammaOptionInput->setStyleClass("dialog-input");
    auto gammaOptionValidator = std::make_shared<WDoubleValidator>();
    gammaOptionValidator->setBottom(0);
    gammaOptionValidator->setTop(100.0);
    gammaOptionInput->setValidator(gammaOptionValidator);
    gammaOptionInput->setMaxLength(4);
    gammaOptionInput->setText(std::to_string(content->getGamma()).substr(0, 4));

    auto thresholdOptionLayout = dialog->contents()->addWidget(std::make_unique<WContainerWidget>())->setLayout(std::make_unique<WHBoxLayout>());
    thresholdOptionLayout->addWidget(std::make_unique<WLabel>("Threshold"), 1);
    auto thresholdOptionInput = thresholdOptionLayout->addWidget(std::make_unique<WLineEdit>());
    thresholdOptionInput->setStyleClass("dialog-input");
    auto thresholdOptionValidator = std::make_shared<WIntValidator>();
    thresholdOptionValidator->setBottom(0);
    thresholdOptionValidator->setTop(100);
    thresholdOptionInput->setValidator(thresholdOptionValidator);
    thresholdOptionInput->setMaxLength(3);
    thresholdOptionInput->setText(std::to_string(content->getThreshold()));

    auto okButton = dialog->footer()->addNew<WPushButton>("OK");
    okButton->setStyleClass("dialog-button");
    okButton->setDefault(true);
    okButton->clicked().connect(dialog.get(), &WDialog::accept);

    auto validator = [okButton, widthOptionInput, gammaOptionInput, thresholdOptionInput]()
    {
        if (widthOptionInput->validate() == ValidationState::Valid && gammaOptionInput->validate() == ValidationState::Valid && thresholdOptionInput->validate() == ValidationState::Valid)
        {
            okButton->enable();
        }
        else
        {
            okButton->disable();
        }
    };

    widthOptionInput->textInput().connect(validator);
    gammaOptionInput->textInput().connect(validator);
    thresholdOptionInput->textInput().connect(validator);

    auto cancelButton = dialog->footer()->addNew<WPushButton>("Cancel");
    cancelButton->setStyleClass("dialog-button");
    cancelButton->clicked().connect(dialog.get(), &WDialog::reject);

    dialog->finished().connect([content = content, dialog = dialog.get(), widthOptionInput, gammaOptionInput, thresholdOptionInput]()
    {
        if (dialog->result() == DialogCode::Accepted)
        {
            content->setPenWidth(std::stoi(widthOptionInput->text()));
            content->setGamma(std::stod(gammaOptionInput->text()));
            content->setThreshold(std::stoi(thresholdOptionInput->text()));
        }
    });

    return dialog;
}

std::string Toolbar::generateUploadFileName()
{
    // File name must be changed every time new image is uploaded and ready to be displayed, because WPaintDevice caches images based on their URI
    std::filesystem::create_directories(sessionId);
    std::string fileName = sessionId + "/in";
    std::filesystem::remove(fileName + std::to_string(uploadFileSuffix));
    ++uploadFileSuffix;
    return fileName + std::to_string(uploadFileSuffix);
}

std::string Toolbar::getUploadFileName() const
{
    return sessionId + "/in" + std::to_string(uploadFileSuffix);
}

std::string Toolbar::generateDownloadFileName()
{
    // File name must be changed every time new colorized image is created and ready to be displayed, because WPaintDevice caches images based on their URI
    std::string fileName = sessionId + "/out";
    std::filesystem::remove(fileName + std::to_string(downloadFileSuffix) + ".png");
    ++downloadFileSuffix;
    return fileName + std::to_string(downloadFileSuffix) + ".png";
}

std::string Toolbar::getDownloadFileName() const
{
    return sessionId + "/out" + std::to_string(downloadFileSuffix) + ".png";
}

Toolbar::Toolbar(Content* content) : content(content), sessionId(WApplication::instance()->sessionId())
{
    setStyleClass("toolbar");

    auto layout = setLayout(createLayout());
    layout->addWidget(createFileMenu());
    layout->addWidget(createEditMenu());
    layout->addWidget(createOptionsButton());
    layout->addWidget(createColorPicker());

    // spacing
    layout->addWidget(std::make_unique<WContainerWidget>(), 1);

    layout->addWidget(createColorizeButton());
    layout->addWidget(createHideButton());
}

Toolbar::~Toolbar()
{
    std::filesystem::remove_all(sessionId);
}

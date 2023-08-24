#include "FileDialogUtils.hpp"

#include <Windows.h>
#include <Commdlg.h>

std::string FileDialogUtils::ShowFileDialog()
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "PNG and JPEG Images\0*.png;*.jpg\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        // File selected, do something with the file path
        // The file path will be stored in ofn.lpstrFile
        return ofn.lpstrFile;
    }
    else
    {
        return "";
    }
}

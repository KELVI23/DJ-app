/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   loopbright_png;
    const int            loopbright_pngSize = 4546;

    extern const char*   loop_png;
    const int            loop_pngSize = 1982;

    extern const char*   stopLoop_png;
    const int            stopLoop_pngSize = 1819;

    extern const char*   playbutton_png;
    const int            playbutton_pngSize = 4750;

    extern const char*   pausebutton_png;
    const int            pausebutton_pngSize = 5057;

    extern const char*   playlist_png;
    const int            playlist_pngSize = 2525;

    extern const char*   load_png;
    const int            load_pngSize = 1530;

    extern const char*   jogwheel_jpeg;
    const int            jogwheel_jpegSize = 154225;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}

#ifndef TERMCOLOR_HPP_
#define TERMCOLOR_HPP_

#include <cstdint>
#include <iostream>

// Detect target's platform and set some macros in order to wrap platform
// specific code this library depends on.
#if defined(_WIN32) || defined(_WIN64)
#   define TERMCOLOR_TARGET_WINDOWS
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#   define TERMCOLOR_TARGET_POSIX
#endif

// If implementation has not been explicitly set, try to choose one based on
// target platform.
#if !defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES) && !defined(TERMCOLOR_USE_WINDOWS_API) && !defined(TERMCOLOR_USE_NOOP)
#   if defined(TERMCOLOR_TARGET_POSIX)
#       define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
#       define TERMCOLOR_AUTODETECTED_IMPLEMENTATION
#   elif defined(TERMCOLOR_TARGET_WINDOWS)
#       define TERMCOLOR_USE_WINDOWS_API
#       define TERMCOLOR_AUTODETECTED_IMPLEMENTATION
#   endif
#endif

// These headers provide isatty()/fileno() functions, which are used for
// testing whether a standard stream refers to the terminal.
#if defined(TERMCOLOR_TARGET_POSIX)
#   include <unistd.h>
#elif defined(TERMCOLOR_TARGET_WINDOWS)
#   include <io.h>
#   include <windows.h>
#endif


namespace termcolor
{
    // Forward declaration of the `_internal` namespace.
    // All comments are below.
    namespace _internal
    {
        inline int colorize_index();
        inline FILE* get_standard_stream(const std::ostream& stream);
        inline FILE* get_standard_stream(const std::wostream& stream);
        template <typename CharT>
        bool is_colorized(std::basic_ostream<CharT>& stream);
        template <typename CharT>
        bool is_atty(const std::basic_ostream<CharT>& stream);

    #if defined(TERMCOLOR_TARGET_WINDOWS)
        template <typename CharT>
        void win_change_attributes(std::basic_ostream<CharT>& stream, int foreground, int background = -1);
    #endif
    }

    template <typename CharT>
    std::basic_ostream<CharT>& reset(std::basic_ostream<CharT>& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
            stream << "\033[00m";
        #elif defined(TERMCOLOR_USE_WINDOWS_API)
            _internal::win_change_attributes(stream, -1, -1);
        #endif
        }
        return stream;
    }

    template <typename CharT>
    std::basic_ostream<CharT>& red(std::basic_ostream<CharT>& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
            stream << "\033[31m";
        #elif defined(TERMCOLOR_USE_WINDOWS_API)
            _internal::win_change_attributes(stream,
                FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }

    template <typename CharT>
    std::basic_ostream<CharT>& green(std::basic_ostream<CharT>& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
            stream << "\033[32m";
        #elif defined(TERMCOLOR_USE_WINDOWS_API)
            _internal::win_change_attributes(stream,
                FOREGROUND_GREEN
            );
        #endif
        }
        return stream;
    }
    namespace _internal
    {
        inline int colorize_index()
        {
            static int colorize_index = std::ios_base::xalloc();
            return colorize_index;
        }

        inline
        FILE* get_standard_stream(const std::ostream& stream)
        {
            if (&stream == &std::cout)
                return stdout;
            else if (&stream == &std::cerr || &stream == &std::clog)
                return stderr;

            return nullptr;
        }

        inline
        FILE* get_standard_stream(const std::wostream& stream)
        {
            if (&stream == &std::wcout)
                return stdout;
            else if (&stream == &std::wcerr || &stream == &std::wclog)
                return stderr;

            return nullptr;
        }

        template <typename CharT>
        bool is_colorized(std::basic_ostream<CharT>& stream)
        {
            return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index()));
        }

        template <typename CharT>
        bool is_atty(const std::basic_ostream<CharT>& stream)
        {
            FILE* std_stream = get_standard_stream(stream);

            if (!std_stream)
                return false;

        #if defined(TERMCOLOR_TARGET_POSIX)
            return ::isatty(fileno(std_stream));
        #elif defined(TERMCOLOR_TARGET_WINDOWS)
            return ::_isatty(_fileno(std_stream));
        #else
            return false;
        #endif
        }

    #if defined(TERMCOLOR_TARGET_WINDOWS)
    

        inline HANDLE get_terminal_handle(std::ostream& stream)
        {
            if (&stream == &std::cout)
                return GetStdHandle(STD_OUTPUT_HANDLE);
            else if (&stream == &std::cerr || &stream == &std::clog)
                return GetStdHandle(STD_ERROR_HANDLE);
            return nullptr;
        }

        inline HANDLE get_terminal_handle(std::wostream& stream)
        {
            if (&stream == &std::wcout)
                return GetStdHandle(STD_OUTPUT_HANDLE);
            else if (&stream == &std::wcerr || &stream == &std::wclog)
                return GetStdHandle(STD_ERROR_HANDLE);
            return nullptr;
        }

        template <typename CharT>
        void win_change_attributes(std::basic_ostream<CharT>& stream, int foreground, int background)
        {
            static WORD defaultAttributes = 0;

            if (!_internal::is_atty(stream))
                return;

            HANDLE hTerminal = INVALID_HANDLE_VALUE;
            hTerminal = get_terminal_handle(stream);

            if (!defaultAttributes)
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                    return;
                defaultAttributes = info.wAttributes;
            }

            if (foreground == -1 && background == -1)
            {
                SetConsoleTextAttribute(hTerminal, defaultAttributes);
                return;
            }

            CONSOLE_SCREEN_BUFFER_INFO info;
            if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                return;

            if (foreground != -1)
            {
                info.wAttributes &= ~(info.wAttributes & 0x0F);
                info.wAttributes |= static_cast<WORD>(foreground);
            }

            if (background != -1)
            {
                info.wAttributes &= ~(info.wAttributes & 0xF0);
                info.wAttributes |= static_cast<WORD>(background);
            }

            SetConsoleTextAttribute(hTerminal, info.wAttributes);
        }
    #endif // TERMCOLOR_TARGET_WINDOWS

    }

}


#undef TERMCOLOR_TARGET_POSIX
#undef TERMCOLOR_TARGET_WINDOWS

#if defined(TERMCOLOR_AUTODETECTED_IMPLEMENTATION)
#   undef TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
#   undef TERMCOLOR_USE_WINDOWS_API
#endif

#endif // TERMCOLOR_HPP_
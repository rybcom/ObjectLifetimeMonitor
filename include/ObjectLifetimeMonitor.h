#pragma once

#include <iostream>
#include <format>
#include <source_location>
#include <filesystem>

#define TRACE(...) set_location(); __VA_ARGS__
#define TRACE_FUNC() FunctionTracing _(__FUNCTION__)

namespace object_lifetime_tracking
{
	
	inline constexpr bool TraceSourceLocation = true;
	
	inline std::source_location source_location;

	enum class Color
	{
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		Gray,
		BrightRed,
		BrightGreen,
		BrightYellow,
		BrightBlue,
		BrightMagenta,
		BrightCyan,
		BrightWhite,
	};

	inline char const* ANSIResetColor = "\x1b[0m";

	inline char const* get_ANSI_escape_characters(Color color)
	{
		switch (color)
		{
		case Color::Black:
			return   "\x1b[30m";
		case Color::Red:
			return   "\x1b[31m";
		case Color::Green:
			return   "\x1b[32m";
		case Color::Yellow:
			return   "\x1b[43m";
		case Color::Blue:
			return   "\x1b[34m";
		case Color::Magenta:
			return   "\x1b[35m";
		case Color::Cyan:
			return   "\x1b[36m";
		case Color::White:
			return   "\x1b[37m";
		case Color::Gray:
			return   "\x1b[90m";
		case Color::BrightRed:
			return   "\x1b[91m";
		case Color::BrightGreen:
			return   "\x1b[92m";
		case Color::BrightYellow:
			return   "\x1b[93m";
		case Color::BrightBlue:
			return   "\x1b[94m";
		case Color::BrightMagenta:
			return   "\x1b[95m";
		case Color::BrightCyan:
			return   "\x1b[96m";
		case Color::BrightWhite:
			return   "\x1b[97m";
		}
		return ANSIResetColor;
	}

	inline void indentStdOutput(int offset  = 0);

	inline void print_colored(std::string_view text, std::string_view appendix, Color color)
	{
		indentStdOutput(1);
		
		if constexpr (TraceSourceLocation)
		{
			auto location_info = std::format("{} [ {}({}:{}) ]",
				source_location.function_name(),
				std::filesystem::path(source_location.file_name()).filename().string(),
				source_location.line(),
				source_location.column());

			std::cout << std::format("{}{:<25} - {:<15}{}{}", get_ANSI_escape_characters(color), text, appendix, location_info, ANSIResetColor);
		}
		else
		{
			std::cout << std::format("{}{:<25} - {:<15}{}", get_ANSI_escape_characters(color), text, appendix, ANSIResetColor);
		}
	}

	inline void print_line_colored(std::string_view text, std::string_view appendix, Color color)
	{
		print_colored(text, appendix, color);
		std::cout << '\n';
	}

	inline void set_location(const std::source_location source = std::source_location::current())
	{
		source_location = source;
	}

	struct FunctionTracing
	{
		std::string function;
		static inline int depth = 0;
		
		FunctionTracing(std::string_view func)
		{
			function = func;
			++depth;
			std::cout << '\n';
			indentStdOutput();
			std::cout << std::format("{}START FUNC :{}", get_ANSI_escape_characters(Color::BrightWhite), function);
			std::cout << ANSIResetColor << '\n';
		}

		~FunctionTracing()
		{
			indentStdOutput();
			std::cout << std::format("{}END FUNC :{}", get_ANSI_escape_characters(Color::BrightWhite), function);
			std::cout << ANSIResetColor << "\n\n";
			--depth;
		}
	};

	inline void indentStdOutput(int offset)
	{
		for (int i = 0; i < FunctionTracing::depth + offset; ++i)
		{
			std::cout << "  ";
		}
	}
	
	struct Sphere
	{
		Sphere() { print_line_colored("Sphere()", "ctor", Color::BrightGreen); }
		Sphere(Sphere const&) { print_line_colored("Sphere(S const&)", "copy ctor", Color::BrightBlue); };
		Sphere(Sphere&&) noexcept { print_line_colored("Sphere(S&&)", "move ctor", Color::BrightCyan); };
		~Sphere() { print_line_colored("~Sphere()", "dtor", Color::BrightRed); }

		Sphere& operator=(Sphere const&)
		{
			print_line_colored("operator=(Sphere const&)", "assign copy", Color::BrightYellow);
			return *this;
		};

		Sphere& operator=(Sphere&&) noexcept
		{
			print_line_colored("operator=(Sphere &&)", "assign move", Color::BrightMagenta);
			return *this;
		};
	};

	struct Cube
	{
		Cube() { print_line_colored("Cube()", "ctor", Color::BrightGreen); }
		Cube(Cube const&) { print_line_colored("Cube(C const&)", "copy ctor", Color::BrightBlue); };
		Cube(Cube&&) noexcept { print_line_colored("Cube(C&&)", "move ctor", Color::BrightCyan); };
		~Cube() { print_line_colored("~Cube()", "dtor", Color::BrightRed); }

		Cube& operator=(Cube const&)
		{
			print_line_colored("operator=(Cube const&)", "assign copy", Color::BrightYellow);
			return *this;
		};

		Cube& operator=(Cube&&) noexcept
		{
			print_line_colored("operator=(Cube &&)", "assign move", Color::BrightMagenta);
			return *this;
		};
	};

	struct Prism
	{
		std::string name;

		Prism(std::string const& naming) : name{ naming } { print_line_colored("Prism() " + name, "ctor", Color::BrightGreen); }
		Prism() : Prism("default") { print_line_colored("Prism() " + name, "ctor", Color::BrightGreen); }
		Prism(Prism const& other)
		{
			name = other.name;
			print_line_colored("Prism(P const&) " + name, "copy ctor", Color::BrightBlue);
		};

		Prism(Prism&& other) noexcept
		{
			name = std::move(other.name);
			print_line_colored("Prism(P&&) " + name, "move ctor", Color::BrightCyan);
		};

		~Prism()
		{
			print_line_colored("~Prism() " + name, "dtor", Color::BrightRed);
		}

		Prism& operator=(Prism const& other)
		{
			name = other.name;
			print_line_colored("operator=(Prism const&) " + name, "assign copy", Color::BrightYellow);
			return *this;
		};

		Prism& operator=(Prism&& other) noexcept
		{
			name = std::move(other.name);
			print_line_colored("operator=(Prism &&) " + name, "assign move", Color::BrightMagenta);
			return *this;
		};
	};
}

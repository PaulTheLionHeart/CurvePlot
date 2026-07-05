# CurvePlot

*CurvePlot is a lightweight mathematical exploration tool for Microsoft Windows. It allows functions, together with their derivatives and integrals, to be plotted interactively.*

Enter almost any mathematical expression and instantly display its graph. CurvePlot can also calculate and display the function's first derivative and indefinite integral, while allowing you to zoom and pan to investigate interesting features in detail.

![CurvePlot Screenshot](images/CurvePlot.png)

---

## Features

- Fast mathematical expression parser
- Plot arbitrary mathematical functions
- Display first derivatives
- Display indefinite integrals
- Interactive zooming and panning
- Adjustable image size
- Built-in sample library
- Native Win32 application (no MFC)
- Copy Image support copies the complete rendered plot to the Windows clipboard, including the graph, legend, and status bar.

---

## Documentation

For a complete guide to using CurvePlot, including examples, zooming, and mathematical exploration, see:

- [User Guide](docs/UserGuide.md)

---

### Formula Library

CurvePlot includes a growing library of sample mathematical functions covering:

- Polynomials
- Trigonometric functions
- Hyperbolic functions
- Exponential and logarithmic functions
- Gaussian and sinc functions
- Logistic and damped oscillations
- Fourier series approximations of common waveforms

Each sample includes a short description to encourage mathematical exploration.

---

## Example Formulae

```text
sin(x)
cos(x)
exp(x)
sqrt(4-x*x)
x^1.5
sin(x*pi)
sin(x)+sin(3*x)/3+sin(5*x)/5+...

```
## Explore Mathematics

CurvePlot isn't just a graphing calculator. You can zoom in and out to investigate the behaviour of functions at different scales, making it useful for studying limits, turning points, discontinuities, asymptotes, derivatives and integrals.

---

## Building

CurvePlot is built with Visual Studio 2017 Community Edition.

Requirements:

- Windows 10 or later
- Visual Studio 2017 or newer

Simply open the solution and build either:

- Debug x64
- Release x64

CurvePlot has no external library dependencies.

---

## Project Structure

```
CurvePlot/
│
├── src/            Application source
├── parser/         Mathematical expression parser
├── docs/           Documentation
└── x64/            Build output
```

---

## History

CurvePlot was originally written while Paul the LionHeart was studying an online course in multivariable calculus. When the lecturer recommended purchasing a graphing calculator, Paul decided to write one instead.

Built using the mathematical expression parser originally developed for Fractint by Mark C. Peterson and later enhanced by Chuck Ebbert, Tim Wegner, and others, CurvePlot reuses many of the interactive ideas developed for ManpWIN, adapting them into a lightweight mathematical exploration tool.

Originally created as a personal learning aid, CurvePlot has since been modernised for today's 64-bit Windows systems while preserving the speed, simplicity, and responsiveness of the original application.

Copy Image copies the complete rendered plot—including the graph, legend, and status bar—to the Windows clipboard.

---

## License

Copyright © 1997–2026 Paul de Leeuw.

The parser retains the original Fractint copyright notice where applicable.

---

## Future Plans

- CMake build system
- Multiple graph support
- Anti-aliased rendering

---

## Credits

**Paul the LionHeart**  
Author, Developer, and Relentless Dragon Tamer

**The Fractint Team**  
For the original mathematical expression parser that inspired and powered CurvePlot for decades.

**ChatGPT**  
Workshop Assistant, Reviewer, Teacher, Sounding Board, and Enthusiastic Dragon Spotter

*"Fun, beauty and passion."*
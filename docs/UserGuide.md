# CurvePlot User Guide

**Version 1.0e**

---

# Welcome to CurvePlot

CurvePlot is a lightweight mathematical exploration tool for Microsoft Windows.

Originally written while studying an online course in multivariable calculus, CurvePlot was created as an alternative to purchasing a commercial graphing calculator. Over time it evolved into a flexible mathematical exploration tool capable of plotting functions together with their derivatives and integrals.

Unlike many graphing calculators, CurvePlot encourages experimentation. Enter a function, zoom into interesting regions, compare it with its derivative or integral, and discover how mathematical functions behave.

---

# Your First Graph

When CurvePlot starts, a collection of sample functions is displayed.

To draw your first graph:

1. Select one of the sample functions.
2. Press **OK**.

Alternatively:

1. Type your own mathematical expression.
2. Press **OK**.

For example:

```
x*x
```

or

```
sin(x)
```

or

```
exp(-x*x)
```

---

# Mouse Controls

CurvePlot is designed to be explored interactively using the mouse.

| Action | Mouse |
|--------|-------|
| Zoom In | Left button drag |
| Zoom Out | Right button drag |

---

## The Function Selection Dialog

![Function Selection Dialog](../images/FunctionSelectionDialogAnnotated.png)

The Function Selection dialog is divided into several areas:

1. **Sample Functions** – Browse the built-in collection of mathematical functions. Selecting a sample copies it into the Formula box, where it can be edited before plotting.
2. **Formula** – Edit the selected function or enter your own mathematical expression.
3. **Description** – Displays a brief explanation of the selected function.
4. **Plots to Display** – Choose whether to display the function, derivative and/or integral. Any combination may be selected.
5. **OK** – Plot the selected function.
6. **Cancel** – Close the dialog without changing the current graph.

---

## Sample Functions

A library of common mathematical functions is provided.

Double-clicking a sample copies it into the Formula box, where it can be modified before pressing OK to plot the function.

## Formula

The Formula box allows any valid mathematical expression supported by the parser.

For example:

```
x*x*x
```

```
sin(x)/x
```

```
exp(-x*x)
```

## Plots to Display

CurvePlot can independently display:

- Function
- Derivative
- Integral

Any combination may be selected.

This makes it easy to study the relationship between a function and its calculus.

---

# Zooming

CurvePlot allows you to explore functions interactively using the mouse.

## Zoom In

1. Press and hold the **left mouse button**.
2. Drag a rectangle around the region you wish to examine.
3. Release the mouse button.

CurvePlot redraws the graph using the selected region.

## Zoom Out

1. Press and hold the **right mouse button**.
2. Drag a rectangle over the current graph.
3. Release the mouse button.

CurvePlot expands the view so that the selected rectangle occupies a smaller portion of the graph, allowing you to explore a wider region.

---

# Copy Image

The **Copy Image** command copies the complete graph to the Windows Clipboard.

The copied image includes:

- Graph
- Legend
- Status bar

making it ideal for reports, presentations and teaching material.

The image is copied in bitmap format and can be pasted directly into most Windows applications.

---

# Exploring Mathematics

The real strength of CurvePlot is not simply drawing graphs—it is helping you explore mathematics visually.

Experiment with different functions, compare them with their derivatives and integrals, and zoom into interesting regions to discover behaviour that may not be obvious from the equations alone.

The following examples are intended as starting points for your own mathematical exploration.

## Quadratic

```
x*x
```

Notice:

- minimum at the origin
- derivative is a straight line
- integral is a cubic

Things to try:

- Zoom near the vertex.
- Compare with `x*x+1`.

---

## Cubic

```
x*x*x
```

Notice:

- point of inflection
- derivative is always non-negative
- integral is a quartic

Things to try:

- Compare with the quadratic.

---

## Sine Wave

```
sin(x)
```

Notice:

- periodic behaviour
- derivative is cosine
- integral is -cosine

Things to try:

- Zoom around π.
- Plot only the derivative.

---

## Gaussian

```
exp(-x*x)
```

One of the most important functions in mathematics.

Used in:

- Statistics
- Physics
- Signal Processing

Things to try:

- Compare the function and its derivative.
- Zoom around the peak.

---

## Sinc Function

```
sin(x)/x
```

A favourite function in signal processing.

Things to explore:

- Zoom around the origin.
- Observe how the oscillations decay.

---

# Tips

- Start with the sample library.
- Zoom into interesting regions.
- Compare functions with their derivatives.
- Turn the integral on and off to study calculus visually.
- Copy graphs directly into reports using Copy Image.
- Double-clicking a sample copies it into the Formula box, allowing it to be modified before pressing **OK**.

---

# Revision History

## Version 1.0e

- Corrected Zoom Out aspect ratio on widescreen displays.
- Corrected initial graph positioning after selecting a new function.
- Expanded the sample function library.
- Added descriptions for all sample functions.
- Improved the Formula Selection dialog.
- Improved clipboard image support.
- Graph, legend and status bar are copied together.
- Added the CurvePlot User Guide.

## Version 1.0d

- Modernised for 64-bit Windows.
- Improved clipboard image support.
- Graph, legend and status bar are copied together.
- Updated documentation.
# CSS Basics

CSS (Cascading Style Sheets) is a language used to describe the presentation of a document written in HTML or XML. It allows you to apply styles to web pages and control the layout of multiple web pages all at once.

## Selecting and Styling

CSS selectors are used to select the HTML elements you want to style. Here are some common types:

### Different Types of Selectors

- **Type Selector**: Selects all elements of a given type.
  ```css
  p {
    color: blue;
  }
  ```
- **Class Selector**: Selects all elements with a given class attribute.

  ```css
  .my-class {
    color: green;
  }
  ```

- **ID Selector**: Selects a single element with a given ID attribute.

  ```css
  #my-id {
    color: red;
  }
  ```

- **Attribute Selector**: Selects elements based on an attribute or attribute value.

  ```css
  a[target="_blank"] {
    color: orange;
  }
  ```

- **Descendant Selector**: Selects all elements that are descendants of a specified element.
  ```css
  div p {
    color: purple;
  }
  ```

## Text and Color in CSS

CSS provides properties to style text and set colors for your web pages.

### Text Properties

- **font-family**: Specifies the font for an element.
  ```css
  p {
    font-family: Arial, sans-serif;
  }
  ```
- **font-size**: Specifies the size of the text.
  ```css
  h1 {
    font-size: 2em;
  }
  ```
- **color**: Specifies the color of the text.
  ```css
  a {
    color: #ff0000;
  }
  ```
- **text-align**: Specifies the horizontal alignment of text.
  ```css
  h2 {
    text-align: center;
  }
  ```
- **text-decoration**: Specifies the decoration added to text.
  ```css
  a {
    text-decoration: none;
  }
  ```

### Color Properties

- **color**: Sets the color of text.
  ```css
  body {
    color: #333333;
  }
  ```
- **background-color**: Sets the background color of an element.
  ```css
  div {
    background-color: #f0f0f0;
  }
  ```

## Box Model Introduction

The CSS box model represents the structure of elements on a web page, including their content, padding, border, and margin.

- **content**: The actual content of the box, where text and images appear.
- **padding**: Clears an area around the content. The padding is transparent.
  ```css
  div {
    padding: 20px;
  }
  ```
- **border**: A border that goes around the padding and content.
  `css
div { border: 1px solid black; }
    `
- **margin**: Clears an area outside the border. The margin is transparent.
  ```css
  div {
    margin: 10px;
  }
  ```

## Document Flow - Block vs. Inline

In CSS, elements can be categorized as either block-level or inline-level.

### Block-level Elements

Block-level elements start on a new line and take up the full width available. Examples include div, h1, and p.

- **display: block;**: Makes an element a block-level element.
  ```css
  div {
    display: block;
  }
  ```

### Inline-level Elements

Inline-level elements do not start on a new line and only take up as much width as necessary. Examples include span, a, and img.

- **display: inline;**: Makes an element an inline-level element.
  ```css
  span {
    display: inline;
  }
  ```

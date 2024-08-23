## Alignment Basics

Alignment in CSS is crucial for controlling the positioning and layout of elements on a web page. It helps create visually appealing designs and ensures content is displayed as intended. Here are some fundamental concepts and properties related to alignment:

### Text Alignment

Text alignment controls the horizontal placement of text within its containing element.

- **text-align**: Specifies the horizontal alignment of text within an element. Possible values are `left`, `right`, `center`, and `justify`.

  Example:
  `p { text-align: center; }`

### Vertical Alignment

Vertical alignment adjusts the vertical positioning of an inline or table-cell element.

- **vertical-align**: Specifies the vertical alignment of an inline or table-cell element. Common values are `baseline`, `top`, `middle`, `bottom`.

  Example:
  `img { vertical-align: middle; }`

### Aligning Block Elements

Block elements can be aligned using margin properties and the `text-align` property of their parent container.

- **margin**: Auto margins can center block elements horizontally.

  Example:
  `.centered-block { margin: 0 auto; width: 50%; }`

### Flexbox Alignment

Flexbox is a powerful layout module that provides more control over alignment and distribution of space among items in a container.

- **justify-content**: Aligns items horizontally within a flex container. Possible values are `flex-start`, `flex-end`, `center`, `space-between`, `space-around`, and `space-evenly`.

  Example:
  `.flex-container { display: flex; justify-content: center; }`

- **align-items**: Aligns items vertically within a flex container. Possible values are `flex-start`, `flex-end`, `center`, `baseline`, and `stretch`.

  Example:
  `.flex-container { display: flex; align-items: center; }`

### Grid Alignment

CSS Grid Layout is another powerful tool for creating complex layouts and aligning items in both horizontal and vertical directions.

- **align-items**: Aligns grid items along the block (vertical) axis.

  Example:
  `.grid-container { display: grid; align-items: center; }`

- **justify-items**: Aligns grid items along the inline (horizontal) axis.

  Example:
  `.grid-container { display: grid; justify-items: center; }`

### Centering with CSS Grid

To center an element within a grid container, use the `place-items` property.

- **place-items**: A shorthand for setting both `align-items` and `justify-items` to the same value.

  Example:
  `.grid-container { display: grid; place-items: center; }`

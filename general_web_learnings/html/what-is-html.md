# What is HTML?

When a team of builders begins constructing a building, one of their key tasks is to assemble the frame. It doesn't matter what building it is; it always starts with a frame of beams and rafters. Once in place, the frame guides the rest of the construction. If you compare a building with a web page, an HTML document is a lot like the frame—it is a basic structure assembled with different components such as tags and elements.

## The Origin of HTML
Sir Tim Berners-Lee, a physicist, often pondered how he and his colleagues from around the world could share information. This question led to the development of HTML. The first version of HTML was released in 1991 along with the first web browser and web server. HTML stands for Hypertext Markup Language.

### What is Hypertext Markup Language?
- **Hypertext:** Text which contains links to other text.
- **Markup:** Refers to tags and elements used within a document.

## Structure of an HTML Document
HTML is simply a text file with a specific structure that consists of elements and tags. HTML files usually have a `.html` suffix. For instance, the first page returned to the browser when you visit a website is often called `index.html`.

### HTML Tags and Elements
Each HTML element consists of an opening tag enclosed in angle brackets. For example, to create a paragraph, you type a left angle bracket `<`, the letter `p` for paragraph, and then a right angle bracket `>`. Most elements are paired with a closing tag, which has a forward slash after the left angle bracket. For example, you close the paragraph element with `<`, `/`, `p`, `>`.

#### Basic Structure of HTML Tags and Elements:
- **Opening tag:** `<p>`
- **Closing tag:** `</p>`
- **Content:** The text or other elements within the tags.

Example:
```html
<p>This is a paragraph.</p>
```

#### Nested Elements
HTML elements can contain other elements. For example, you can add an italics element inside a paragraph element to make text appear in italics.

```
<p>This is a <i>paragraph</i> with italics.</p>
``` 

#### Self-Closing Tags
Elements can also be empty or self-closing, meaning they do not have a closing tag. One example of a self-closing element is the line break tag.

```
<p>This is a paragraph.<br>And this is a new line within the same paragraph.</p>
```

Self-closing tags can also be written with a forward slash before the right angle bracket:

```
<br />
```
### HTML Standards and Specifications
The rules and structure for elements and tags are known as the HTML specification. The HTML specification is maintained by the World Wide Web Consortium (W3C). Whenever the HTML specification changes, a new version of HTML is standardized. The current version is HTML5.

### Summary
HTML elements with their opening and closing tags and angle brackets build up an HTML document. These elements form the structure of a web page and describe to the web browser what to display. For example, the browser reads an HTML page that has image tags to display an image file called icon.png, and a p tag to display a paragraph under that image.

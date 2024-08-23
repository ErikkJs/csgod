# Networking and Web Technologies Study Guide

## Understanding IP Addresses and Data Transmission

### The Postal System Analogy
Email is a common communication method that we all know about. Before email, the primary alternative was sending mail through the postal system. Interestingly, there is a surprising parallel between the postal system and how computers send and receive data across the internet.

In the postal system, you need an address to send a letter to a recipient. Similarly, computers use IP addresses to send data across the internet. IP addresses function much like postal addresses, making it possible for packets of information to be delivered to the correct destination.

### What are IP Addresses?
An IP (Internet Protocol) address is a unique identifier assigned to each computer on a network. It allows data to be sent from one computer to another. There are currently two widely used versions of Internet Protocol: IPv4 and IPv6.

- **IPv4:** An IPv4 address contains four octets separated by periods. For example, `192.0.2.235`.
- **IPv6:** An IPv6 address contains eight groups of hexadecimal digits separated by colons. For example, `4527:0a00:1567:0200:ff00:0042:8329`.

### How Data is Sent Across the Internet
When you send data across a network, it is transmitted as a series of messages called IP packets (or datagrams). Each IP packet consists of a header and a payload.

- **Header:** Contains the source IP address, the destination IP address, and additional information to help deliver the packet.
- **Payload:** Contains the actual data being sent, along with other protocols that handle the data.

### Common Issues and Protocols to Solve Them
Just like with the postal system, things can go wrong with data transmission over the internet. IP packets can arrive out of order, become damaged or corrupted, or be dropped and lost during transit. To address these issues, additional protocols are used within the payload of IP packets.

- **Transmission Control Protocol (TCP):** Ensures data is delivered correctly and in order. It solves issues related to out-of-order delivery, corruption, and loss, but it can introduce a small delay. TCP is used for data that must be accurate and complete, such as text and image files.
- **User Datagram Protocol (UDP):** Solves the issue of corrupted packets but does not guarantee the order or delivery of packets. It is used for data where some loss is acceptable, such as voice calls or live video streaming.

By understanding these key concepts, you can now explain how IP addresses work and how computers send data across the internet, similar to how the postal system operates.

## Additional Key Concepts

### TCP (Transmission Control Protocol)
TCP is a core protocol of the Internet Protocol Suite. It ensures reliable, ordered, and error-checked delivery of data between applications running on hosts communicating via an IP network. Unlike other protocols, TCP ensures that data packets are delivered in the correct order.

### HTTP Methods
HTTP (Hypertext Transfer Protocol) defines several methods that indicate the desired action to be performed for a given resource. Common HTTP methods include:
- **GET:** Requests data from a server.
- **POST:** Sends data to a server to create or update a resource.
- **DELETE:** Removes a resource from a server.
Methods like **SEND** are not valid in the context of HTTP.

### HTML (HyperText Markup Language)
HTML is the standard markup language used to create web pages. It describes the structure and content of a web page using elements such as headings, paragraphs, links, and images. HTML files are static and display the same content every time they are accessed.

### Domain Name System (DNS)
DNS is a protocol within the set of standards for how computers exchange data on the Internet and on many private networks. It translates human-friendly domain names (like coursera.org) into IP addresses that computers use to identify each other on the network. This process is essential for routing traffic on the internet.

### Websites vs. Web Applications
- **Websites:** Generally static and primarily provide information. The content does not change based on user interactions.
- **Web Applications:** Interactive and dynamic, offering functionalities such as user input and personalized content. They often require user authentication and can provide a richer user experience compared to static websites.



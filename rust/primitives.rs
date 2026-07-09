fn main() {
    // **Signed Integers**
    // Signed integers can store both positive and negative values.

    let a: i8 = -128;     // 8-bit signed integer: Range -128 to 127
    let b: i16 = -32768;  // 16-bit signed integer: Range -32,768 to 32,767
    let c: i32 = -2147483648; // 32-bit signed integer: Range -2,147,483,648 to 2,147,483,647
    let d: i64 = -9223372036854775808; // 64-bit signed integer: Range -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
    let e: i128 = -170141183460469231731687303715884105728; // 128-bit signed integer: Very large range
    let f: isize = -42;   // Architecture-dependent signed integer: 32-bit on 32-bit systems, 64-bit on 64-bit systems

    println!("Signed integers: {}, {}, {}, {}, {}, {}", a, b, c, d, e, f);

    // **Unsigned Integers**
    // Unsigned integers can store only positive values (including zero).

    let ua: u8 = 255;      // 8-bit unsigned integer: Range 0 to 255
    let ub: u16 = 65535;   // 16-bit unsigned integer: Range 0 to 65,535
    let uc: u32 = 4294967295; // 32-bit unsigned integer: Range 0 to 4,294,967,295
    let ud: u64 = 18446744073709551615; // 64-bit unsigned integer: Range 0 to 18,446,744,073,709,551,615
    let ue: u128 = 340282366920938463463374607431768211455; // 128-bit unsigned integer: Very large range
    let uf: usize = 42;    // Architecture-dependent unsigned integer: 32-bit on 32-bit systems, 64-bit on 64-bit systems

    println!("Unsigned integers: {}, {}, {}, {}, {}, {}", ua, ub, uc, ud, ue, uf);

    // **Floating-Point Numbers**
    // Floating-point numbers can represent fractional values.

    let fa: f32 = 3.14159; // 32-bit floating-point number (single precision)
    let fb: f64 = 2.718281828459045; // 64-bit floating-point number (double precision, more precise and default for floats in Rust)

    println!("Floating-point numbers: {}, {}", fa, fb);

      // **String Slices (`&str`)**
    // A string slice is a reference to a sequence of UTF-8 encoded characters within a string.
    // It is immutable and does not own the data; it's often used for borrowed views into strings.

    let s: &str = "Hello, world!";  // A string slice, immutable and borrowed
    let part: &str = &s[0..5];      // Slicing a string slice: "Hello"
    println!("String slice: {}", s);
    println!("Part of string slice: {}", part);

    // **String (`String`)**
    // A heap-allocated, growable string type in Rust.
    // It owns its data and can be mutated.

    let mut growable = String::from("Hello"); // Creating a String from a string literal
    growable.push_str(", world!");            // Appending to the String
    println!("Growable string: {}", growable);

    // **Converting between `String` and `&str`**
    // Converting a String to a &str can be done with a reference.
    // Converting a &str to a String involves creating a new String instance.

    let s_slice: &str = &growable;  // Borrowing a String as a string slice
    let s_string: String = s_slice.to_string(); // Creating a String from a string slice
    println!("Converted &str to String: {}", s_string);

    // **Common Operations with Strings**
    // Rust strings support various operations, including concatenation, slicing, and iteration.

    // Concatenation
    let concat = format!("{} - {}", s, growable); // Using format! macro to concatenate
    println!("Concatenated string: {}", concat);

    // Iteration over characters
    for c in s.chars() {
        println!("Character: {}", c);  // Iterating over characters in a string slice
    }

    // **String Escaping and Special Characters**
    let special = "Rust 🦀 \n\tSpecial characters!"; // Using Unicode and escape sequences
    println!("String with special characters: {}", special);

    // **Multiline Strings**
    // Rust allows defining multiline strings using raw string literals with `r#`.

    let multiline = r#"This is a
    multiline string
    with "quotes" and other special characters like \."#;
    println!("Multiline string: {}", multiline);
}

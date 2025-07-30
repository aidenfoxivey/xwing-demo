import sys

if len(sys.argv) != 3:
    print("Usage: python convert.py <hex_string> <output_string>")
    exit()


hex_string = sys.argv[1]
var_name = sys.argv[2]

# Ensure the string is of even length
if len(hex_string) % 2 != 0:
    print("Error: Hex string has an odd length.")
    exit()

byte_values = []
for i in range(0, len(hex_string), 2):
    byte_hex = hex_string[i:i+2]
    byte_val = int(byte_hex, 16)
    byte_values.append(byte_val)

# Determine the array size
array_size = len(byte_values)

# Generate the C++ std::array declaration
cpp_array_declaration = f"std::array<uint8_t, {array_size}> {var_name} = {{\n"
cpp_array_declaration += "    " # Indent for readability

for i, val in enumerate(byte_values):
    cpp_array_declaration += f"0x{val:02x}" # Format as 0xYY hex
    if i < len(byte_values) - 1:
        cpp_array_declaration += ", "
    if (i + 1) % 8 == 0 and (i + 1) < len(byte_values): # New line every 8 bytes for readability
        cpp_array_declaration += "\n    "

cpp_array_declaration += "\n};"

print(cpp_array_declaration)

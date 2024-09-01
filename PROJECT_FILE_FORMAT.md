# .fdp file format specification

## More about the format:
<span style="color: red;">__it's little endian hehehehe__</span>

## <span style="color: blue;">The header:</span>

| Type | Description |
| ---- | ----------- |
| str  | Must be "FDProj". If it's not this, isn't an .fdp file |
| int  | Version major |
| int  | Version minor |
| long | Length of animation in frames |
| int | Animation width |
| int | Animation height |
| int | Animation FPS |
| str | Color space eg. sRGB, terminating with '.' |
| str | Animation display name, terminating with byte 0 |
| str | Animation description, terminating with byte 0  |
| str | Animation author, terminating with byte 0 |

## <span style="color: cyan;">Palette</span>
For each color in the palette, this is the structure:
| Type | Description |
| ---- | ----------- |
| long | color index |
| int | r |
| int | g |
| int | b |


at the end, this happens:
| Type | Description |
| ---- | ----------- |
| str | Constant: "PALEND" |


## The actual format
The .fdp file format uses this structure per frame to describe frames:
| Type | Description |
|-------|------------|
| long  | Frame id: eg. 0,1,2,3,4,5... |
| [colorarr](#color-arrays) | Colors (each pixel) |

## Color Arrays 
An array variant which is used to describe palette colors: 
It's just continuous longs which are palette color indices.
The end is marked by the string "COLOREND"

Also the origin for colors in frames is the top-left corner.

# <span style="color: red;">The END</span>
| Type | Description |
| ---- | ----------- |
| str | A constant of EOP (end of project)


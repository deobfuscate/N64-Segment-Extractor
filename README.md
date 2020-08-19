# N64 Segment Extractor
N64 homebrew tool to extract segments of binary ROM files using symbol tables.


## Usage:
`extractseg <binary file> <symbol to extract>`, symbol table as stdin
## Example:
`nm game_sym.out | exctactseg game.n64 code`

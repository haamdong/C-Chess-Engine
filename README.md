<div align="center">

<h1>C-Chess-Engine</h1>

<p>
A classical chess engine implemented in pure C
</p>

<p>
<img src="https://img.shields.io/badge/Language-C-blue.svg" />
<img src="https://img.shields.io/badge/Status-Development-orange.svg" />
<img src="https://img.shields.io/badge/Type-Chess%20Engine-black.svg" />
</p>

</div>

<hr>

<h2>About The Project</h2>

<p>
C-Chess-Engine is a lightweight, modular chess engine written entirely in C.
The project focuses on implementing the fundamental components of a classical
search-based chess engine, including move generation, evaluation, and tree search.
</p>

<hr>

<h2>Core Architecture</h2>

<table>
<tr>
<th align="left">Component</th>
<th align="left">Description</th>
</tr>

<tr>
<td><b>Board Representation</b></td>
<td>Maintains the 8x8 board state, piece encoding, and turn control.</td>
</tr>

<tr>
<td><b>Move Generation</b></td>
<td>Generates pseudo-legal moves and filters illegal moves that expose the king.</td>
</tr>

<tr>
<td><b>Search</b></td>
<td>Implements depth-limited Minimax enhanced with Alpha-Beta pruning.</td>
</tr>

<tr>
<td><b>Evaluation</b></td>
<td>Material-based scoring with optional positional heuristics.</td>
</tr>

</table>

<hr>

<h2>Features</h2>

<ul>
<li>Standard 8x8 chess board implementation</li>
<li>Legal move validation</li>
<li>Material evaluation system</li>
<li>Minimax search algorithm</li>
<li>Alpha-Beta pruning optimization</li>
<li>Modular C codebase</li>
<li>Command-line interface</li>
</ul>

<hr>

<h2>Build & Run</h2>

<h3>Requirements</h3>
<ul>
<li>GCC or Clang compiler</li>
</ul>

<h3>Compile</h3>

<pre>
gcc -o chess main.c board.c move.c search.c evaluate.c
</pre>

<h3>Execute</h3>

<pre>
./chess
</pre>

<hr>

<h2>Engine Workflow</h2>

<ol>
<li>Initialize board state</li>
<li>Generate legal moves</li>
<li>Evaluate candidate positions</li>
<li>Search using Minimax + Alpha-Beta</li>
<li>Select optimal move</li>
</ol>

<hr>

<h2>Evaluation Model</h2>

<table>
<tr>
<th align="left">Piece</th>
<th align="right">Value</th>
</tr>
<tr><td>Pawn</td><td align="right">100</td></tr>
<tr><td>Knight</td><td align="right">320</td></tr>
<tr><td>Bishop</td><td align="right">330</td></tr>
<tr><td>Rook</td><td align="right">500</td></tr>
<tr><td>Queen</td><td align="right">900</td></tr>
<tr><td>King</td><td align="right">20000</td></tr>
</table>

<hr>

<h2>Planned Enhancements</h2>

<ul>
<li>UCI protocol support</li>
<li>Transposition tables (Zobrist hashing)</li>
<li>Iterative deepening</li>
<li>Move ordering heuristics</li>
<li>Quiescence search</li>
<li>Opening book integration</li>
<li>Endgame tablebases</li>
</ul>

<hr>

<h2>Project Structure</h2>

<pre>
C-Chess-Engine
│
├── main.c
├── board.c / board.h
├── move.c / move.h
├── search.c / search.h
├── evaluate.c / evaluate.h
└── README.md
</pre>

<hr>

<div align="center">

<h3>Educational Chess Engine Project</h3>
<p>Designed for understanding classical AI search techniques in C.</p>

</div>

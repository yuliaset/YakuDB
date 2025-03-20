# YakuDB
This program enumerates all possible 14-tile Mahjong hands, separates them into categories and then writes them to an HDF5 file (dataset.h5). Each category is stored in a separate dataset in the .h5 file:

- standardWinningHands
- chiitoiHands
- kokushiHands

**main.c**: 
- Calculates total possible 14-tile hands (with up to 4 copies of each tile).
- Enumerates specific winning configurations:
- Standard hands (4 melds + 1 pair).
- Chiitoi (7 distinct pairs).
- Kokushi Musou (13 unique terminal/honor tiles + 1 duplicate).
- Populates arrays in memory with each winning hand.
- Creates an HDF5 file (dataset.h5) and writes the arrays into datasets

**Requirements**:
- HDF5 Library

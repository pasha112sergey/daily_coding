from enum import Enum




class PieceType(Enum):
    PAWN   = 0
    KNIGHT = 1
    BISHOP = 2
    ROOK_HAS_MOVED   = 3
    ROOK_HAS_NOT_MOVED = 4
    QUEEN  = 5
    KING_HAS_MOVED = 6
    KING_HAS_NOT_MOVED = 7

# 6 unique types [piecetype][color]
# 4 bits total to represent a piece uniquely

# 320 
# 64 * 4 = 256 

class PieceColor(Enum):
    WHITE = 1
    BLACK = 0

class Piece():
    def __init__(self, pieceType, pieceColor):
        self.type = PieceType(pieceType)
        self.color = pieceColor
    
    def binRepresentation(self):
        match self.type:
            case PieceType.PAWN:
                return "000"
            case PieceType.KNIGHT:
                return "001"
            case PieceType.ROOK_HAS_MOVED:
                return "010"
            case PieceType.ROOK_HAS_NOT_MOVED:
                return "011"
            case PieceType.QUEEN:
                return "100" 
            case PieceType.KING_HAS_MOVED:
                return "101"
            case PieceType.KING_HAS_NOT_MOVED:
                return "111"
            case _:
                assert False

    def __str__(self):
        color = "W=" if self.color == PieceColor.WHITE else "B="
        match self.type:
            case PieceType.PAWN:
                return "P" + color
            case PieceType.KNIGHT:
                return "N" + color
            case PieceType.ROOK_HAS_MOVED:
                return "R-" + color
            case PieceType.ROOK_HAS_NOT_MOVED:
                return "R+" + color
            case PieceType.QUEEN:
                return "Q" + color
            case PieceType.KING_HAS_MOVED:
                return "K-" + color
            case PieceType.KING_HAS_NOT_MOVED:
                return "K+" + color
            case _:
                return "NA"


class Square():
    def __init__(self, occupied=False, occupiedBy=None):
        self.occupied = occupied
        self.occupiedby = occupiedBy 
    
    def __str__(self):
        if self.occupiedby == None:
            return "-"
        return str(self.occupiedby)



class Chessboard():
    def __init__(self):
        # each element in the list 
        # list of 8 rows by 8 columns of squares
        self.playerTurn = PieceColor.WHITE
        self.board = []

        for i in range(8):
            self.board.append([])
            for j in range(8):
                self.board[i].append(Square())
        
        for i in range(8):
            self.board[6][i] = Square(True, Piece(PieceType.PAWN, PieceColor.WHITE))
            self.board[1][i] =  Square(True, Piece(PieceType.PAWN, PieceColor.BLACK))

    def __str__(self):
        s = ""
        for row in self.board:
            for piece in row:
                s += str(piece) + " "
            s += "\n"
        return s

    def store(self): 
        # each square is represented by 4 bits
        # bit format: occupied (1 true 0 false), color (1 white, 0 black), binary representation of the piece value
        out = ""
        for row in self.board:
            for sq in row:
                if sq.occupied:
                    out += "1"
                else:
                    out += "00000"
                    continue

                if sq.occupiedby.color == PieceColor.WHITE:
                    out+="1"
                else:
                    out+="0"
                
                out += sq.occupiedby.binRepresentation()

        return out
    

    def load(self, bits): # string bits -> convert to the actual board
        i = 0
        newSquares = []

        while i < len(bits):
            occupied  = int(bits[i  ])
            color     = int(bits[i+1])

            pieceBit1 = int(bits[i+2])
            pieceBit2 = int(bits[i+3])
            pieceBit3 = int(bits[i+4])

            pieceType = pieceBit1 | (pieceBit2 << 1) | (pieceBit3 << 2)
            
            sq = None
            if not occupied:
                sq = Square()
            
            else:
                p = Piece(pieceType, color)
                sq = Square(occupied = True, occupiedBy=p)
            
            newSquares.append(sq)

            # print(occupied, color, pieceType)
            i+=5
        
        for i in range(64):
            self.board[i // 8][i % 8] = newSquares[i]
        

# function that initializes a chessboard

def main():
    board = Chessboard()
    print("before:", board)
    s = board.store()
    print('\n', s)
    board.load(s)
    print("after: \n", board)

main()

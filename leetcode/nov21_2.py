from enum import Enum

class Tree(object):
    def __init__(self, x):
        self.value = x
        self.left = None
        self.right = None
    
    def __str__(self):
        return f"{self.value}: left: {self.left}; right: {self.right}"
    


def helper(root, count):
    if root == None:
        return count
    
    if (root.value == "🦀"):
        count += 1
    
    return max(helper(root.left, count), helper(root.right, count))


def solution(root):
    return helper(root, 0)


def create_tree(d):

    if d == None:
       return None
    root = Tree(d["value"])
    root.left = create_tree(d["left"])
    root.right = create_tree(d["right"])

    return root



def main():
   d = d = {
        "value": "💯",
        "left": {
            "value": "🤡",
            "left":
            {
                "value": "🦀",
                "left": None,
                "right": None
            },
            "right": 
            {
                "value": "100",
                "left": None,
                "right": {
                    "value": "😃",
                    "left": None,
                    "right": None
                }
            }
        },
        "right": {
            "value": "🦀",
            "left":
            {
                "value": "🦀",
                "left": None,
                "right": {
                    "value": "🦀",
                    "left": None,
                    "right": None
                }
            },
            "right": 
            {
                "value": "🦀",
                "left": None,
                "right": None
            }
        }
    }
   
   test_tree = create_tree(d)
#    print(test_tree)
   ans = solution(test_tree)
   print(ans)

main()

class PieceType(Enum):
    PAWN   = 1
    KNIGHT = 2
    BISHOP = 3
    ROOK   = 4
    QUEEN  = 5
    KING   = 6

# 6 unique types [piecetype][color]
# 4 bits total to represent a piece uniquely

# 320 
# 64 * 4 = 256

class PieceColor(Enum):
    WHITE = 0
    BLACK = 1

class Piece():
    def __init__(self, pieceType, pieceColor):
        self.type = pieceType
        self.color = pieceColor

class Square():
    def __init__(self):
        self.occupied = False
        self.occupiedby = None 

class Chessboard():
    def __init__(self):
        # each element in the list 
        # list of 8 rows by 8 columns of squares
        self.board = [[Square()] * 8] * 8 
import { useState, useEffect } from "react";

export default function Game() {
    const [history, setHistory] = useState([Array(9).fill(null)]);
    const [turn1, setTurn] = useState(true);
    const [step, setStep] = useState(0);

    const currentSquares = history[step];

    function handlePlay(nextSquares) {
        setHistory((h) => [...h.slice(0, step + 1), nextSquares]);
        setStep((s) => s + 1);
        setTurn((turn) => !turn1);
    }

    function resetGame() {
        setTurn(true);
        setHistory([...history, Array(9).fill(null)]);
        setStep(0);
    }

    function goBack() {
        setStep((s) => Math.max(0, s - 1));
        setTurn((t) => !t);
    }

    return (
        <div>
            <div>
                <Board
                    turn1={turn1}
                    squares={currentSquares}
                    onPlay={handlePlay}
                    reset={resetGame}
                    goBack={goBack}
                />
            </div>
        </div>
    );
}

export function Board({ turn1, squares, onPlay, reset, goBack }) {
    const [scoreX, setScoreX] = useState(0);
    const [scoreO, setScoreO] = useState(0);

    const winner = calculateWinner(squares);
    let winnerStatus;

    useEffect(() => {
        if (winner) {
            if (winner == "X") setScoreX(scoreX + 1);
            else setScoreO(scoreO + 1);
        }
    }, [winner]);

    if (winner) winnerStatus = "Winner: " + winner + "!";
    else winnerStatus = "Next player: " + (turn1 ? "X" : "O");

    function handleClick(i) {
        if (calculateWinner(squares) || squares[i]) {
            return;
        }
        const nextSquares = squares.slice();
        if (turn1) {
            nextSquares[i] = "X";
        } else {
            nextSquares[i] = "O";
        }

        onPlay(nextSquares);
    }

    return (
        <>
            <div>{winnerStatus}</div>
            <div className="board-row">
                <Square
                    value={squares[0]}
                    onSquareClick={() => handleClick(0)}
                />
                <Square
                    value={squares[1]}
                    onSquareClick={() => handleClick(1)}
                />
                <Square
                    value={squares[2]}
                    onSquareClick={() => handleClick(2)}
                />
            </div>
            <div className="board-row">
                <Square
                    value={squares[3]}
                    onSquareClick={() => handleClick(3)}
                />
                <Square
                    value={squares[4]}
                    onSquareClick={() => handleClick(4)}
                />
                <Square
                    value={squares[5]}
                    onSquareClick={() => handleClick(5)}
                />
            </div>
            <div className="board-row">
                <Square
                    value={squares[6]}
                    onSquareClick={() => handleClick(6)}
                />
                <Square
                    value={squares[7]}
                    onSquareClick={() => handleClick(7)}
                />
                <Square
                    value={squares[8]}
                    onSquareClick={() => handleClick(8)}
                />
            </div>
            <button onClick={() => reset()}>Reset game</button>
            <button onClick={() => goBack()}>Go Back</button>
            <div>Score: {scoreX + " : " + scoreO}</div>
        </>
    );
}

function Square({ value, onSquareClick }) {
    return (
        <button className="square" onClick={onSquareClick}>
            {value}
        </button>
    );
}

function calculateWinner(squares) {
    const lines = [
        [0, 1, 2],
        [3, 4, 5],
        [6, 7, 8],
        [0, 3, 6],
        [1, 4, 7],
        [2, 5, 8],
        [0, 4, 8],
        [2, 4, 6],
    ];

    for (let i = 0; i < lines.length; i++) {
        const [a, b, c] = lines[i];
        if (
            squares[a] &&
            squares[a] === squares[b] &&
            squares[a] == squares[c]
        ) {
            return squares[a];
        }
    }
    return null;
}

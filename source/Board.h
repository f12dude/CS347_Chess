#ifndef _BOARD_
#define _BOARD_

#include "Piece.h"
#include "Move.h"
#include "vec2.h"
#include "BoardMove.h"
#include <unordered_map>
#include <functional>

#include <vector>

struct BoardPiece
{
	Piece piece;
	int owner;
	int file;
	int rank;
	int hasMoved;
	int type;
};

// Moves, then unmoves a piece move upon destruction
class ApplyMove
{
public:

	ApplyMove(const BoardMove* pMove, class Board* pBoard);
	~ApplyMove();

private:

	const BoardMove* m_pMove;
	Board* m_pBoard;

	int m_oldIndex;
	int m_newIndex;
	int m_hasMoved;
	BoardMove m_LastMove;
	ivec2 m_oldKingPos;
};

// Defines a chess board which manages generating valid action states
class Board
{
public:

	friend class ApplyMove;

	// Constructs an empty board
	Board();

	// Updates the grid and returns all valid moves
	void Update(const Move* pLastMove, std::vector<Piece>& pieces);

	std::vector<BoardMove> GetMoves(int playerID);

	// Returns the piece at pos
	// If there is not a piece at pos, nullptr is returned
	BoardPiece* GetPiece(const ivec2& pos);
	const BoardPiece* GetPiece(const ivec2& pos) const;

private:

	float GetWorth(int playerID);

	std::vector<BoardMove> GetMoves(int playerID, bool bCheck);

	// Generate valid moves for pawns
	void GeneratePawnMoves(const BoardPiece& piece, bool bCheck, std::vector<BoardMove>& moves);

	// Generates valid moves for pawns that have the possilbity of being promoted
	void GeneratePromotedPawnMoves(const ivec2& from, const ivec2& to, int playerID, bool bCheck, std::vector<BoardMove>& moves);

	// Generates valid moves for bishops rooks and queens
	void GenerateDirectionMoves(const BoardPiece& piece, bool bCheck, std::vector<BoardMove>& moves);

	// Generates valid moves for knights and kings
	void GenerateDiscreteMoves(const BoardPiece& piece, bool bCheck, std::vector<BoardMove>& moves);

	// Generates valid castle moves
	void GenerateCastleMove(const BoardPiece& piece, bool bCheck, std::vector<BoardMove>& moves);

	// Adds a move to the move list only if after applying the move, it does not put us in check, or if bCheck is false
	void AddMove(const BoardMove& move, bool bCheck, std::vector<BoardMove>& moves);

	// Returns true if pos is on the board
	bool IsOnBoard(int pos) const;

	// Returns true if pos is on the board
	bool IsOnBoard(const ivec2& pos) const;

	// Returns true if the current tile is empty
	bool IsTileEmpty(int file, int rank) const;

	// Returns true if we currently own the tile
	bool IsTileOwner(int file, int rank, int playerID) const;

	// Returns true if the current state of the board is in check
	bool IsInCheck(int playerID);

	bool IsInCheckmate(int playerID);

	// Clears the board
	void Clear();

private:

	std::vector<std::vector<int>> m_board;
	std::unordered_map<int,BoardPiece> m_pieces;

	ivec2 m_kingPos[2];
	BoardMove m_LastMove;
};

#endif // _BOARD_

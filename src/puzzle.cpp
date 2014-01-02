#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "puzzle.h"
#include "exception.h"

Puzzle::Puzzle()
{
}

QList<Puzzle> Puzzle::Parse(QString filename)
{
    QList<Puzzle> puzzles;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        throw Exception("open file error: " + file.errorString().toStdString());
    }

    QTextStream in(&file);

    bool isHeaderRow = true;
    while(!in.atEnd()) {
        QString line = in.readLine();

        // skip first row - it is header
        if (isHeaderRow)
        {
            isHeaderRow = false;
            continue;
        }

        Puzzle puzzle = Read(line);
        puzzles.append(puzzle);
    }

    file.close();

    return puzzles;
}

QList<QString> Puzzle::TokenizeLine(QString line)
{
    QList<QString> tokens;
    QString currentToken;
    int position = 0;
    bool inQuotes = false;

    while (position < line.length())
    {
        QChar currentChar = line.at(position);

        if (currentChar == '"')
        {
            inQuotes = !inQuotes;
        } else if (currentChar == ',' && !inQuotes)
        {
            tokens.append(currentToken);
            currentToken = "";
        } else
        {
            currentToken += currentChar;
        }

        position += 1;
    }

    // append last token
    tokens.append(currentToken);

    return tokens;
}

Puzzle Puzzle::Read(QString line)
{
    Puzzle puzzle;

    // id,authors,source,date,stipulation,fen
    QList<QString> tokens = TokenizeLine(line);

    if (tokens.count() != 6)
    {
        throw Exception(QString("Expected 6 tokens in line: '%1', but actual %2").arg(line).arg(tokens.count()).toStdString());
    }

    puzzle.Id = tokens[0].toInt();
    puzzle.Authors = tokens[1];
    puzzle.Source = tokens[2];
    puzzle.Date = QDate::fromString(tokens[3], "M/d/yyyy");
    puzzle.PositionFEN = tokens[5];

    QString stiputation = tokens[4];
    if (stiputation == "#2") // mate in two, now it only supported type
    {
        puzzle.Goal = PuzzleGoal::MateInTwo;
    } else
    {
        throw Exception("Unknown stiputation: " + stiputation.toStdString());
    }

    return puzzle;
}



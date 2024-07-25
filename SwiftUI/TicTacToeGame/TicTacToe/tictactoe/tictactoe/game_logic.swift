//
//  game_logic.swift
//  tictactoe
//
//  Created by Joshua Guillen on 7/24/24.
//

import Foundation

enum Player{
    case X
    case O
}

class ticTacModel: ObservableObject{
    @Published var board:[Player?] = Array(repeating: nil, count:9)
    @Published var activePlayer:Player = .X
    @Published var winner:Player? = nil
    
    //Button is pressed
    func buttonTap(i:Int){
        guard board[i] == nil && winner == nil else{
            return
            
        }
        board[i] = activePlayer
        if checkWinner(){
            winner = activePlayer
            print("\(activePlayer) Has won the game!")
        } else{
            activePlayer = (activePlayer == .X) ? .O : .X
        }
       
    }
    
    //Label of functionion
    
    func buttonLabel(i : Int) -> String {
        if let Player = board[i]{
            return Player == .X ? "X" : "O"
        }
        return ""
        
    }
    
    //Reset Game
    func resetGame() {
        board =  Array(repeating: nil, count:9)
        activePlayer = .X
        winner = nil
        
    }
    
    //Check For Winner
    func checkWinner() -> Bool {
        //rows
        for i in stride(from: 0, to: 9, by: 3){
            if board[i] == activePlayer && board[i+1] == activePlayer && board[i+2] == activePlayer{
                return true
            }
        }
        
        //columns
        for i in 0..<3{
            if board[i] == activePlayer && board[i+3] == activePlayer && board[i+6] == activePlayer{
                return true
            }
        }
        //diagonal left
        if board[0] == activePlayer && board[4] == activePlayer  && board[8] == activePlayer{
            return true
        }
        //diagonal right
        if board[2] == activePlayer && board[4] == activePlayer  && board[6] == activePlayer{
            return true
        }

        return false
    }
    
    
}


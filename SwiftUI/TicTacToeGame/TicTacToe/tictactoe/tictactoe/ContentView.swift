//
//  ContentView.swift
//  tictactoe
//
//  Created by Joshua Guillen on 7/19/24.
//

import SwiftUI

struct ContentView: View {
    
    @ObservedObject var TicTac = ticTacModel()
    @State private var title = "TIC TAC TOE"
    
    var body: some View {
        VStack {
            Text(title)
                .font(.system(size: 45, weight: .heavy))
            let col = Array(repeating: GridItem(.flexible()), count: 3)
            
            LazyVGrid(columns: col, content: {
                ForEach(0..<9){ i in
                    Button(action: {
                        TicTac.buttonTap(i: i)
                        if TicTac.checkWinner() {
                            title = "\(TicTac.activePlayer) has won!"
                        }
                        
                    }, label: {
                        Text(TicTac.buttonLabel(i: i))
                            .frame(width: 100, height: 100)
                            .background(.blue)
                            .foregroundStyle(.white)
                            .font(.system(size: 45, weight: .heavy))
                    })
                    
                }
                
            })
            .padding(.bottom)
            Button(action: {
                TicTac.resetGame()
                title = "TIC TAC TOE"
                
            }, label: {
                Text("RESTART")
                    .frame(width: 180, height: 50)
                    .background(.blue)
                    .foregroundStyle(.white)
                    .font(.system(size: 30, weight: .heavy))
                    .clipShape(.capsule)
            })
            Spacer()
            Text("Joshua Guillen Project — 2024")
                .font(.system(size: 14))
                .foregroundColor(.gray)
                .padding(.bottom, 10)
            
        }
        .padding()
    }
}

#Preview {
    ContentView()
}

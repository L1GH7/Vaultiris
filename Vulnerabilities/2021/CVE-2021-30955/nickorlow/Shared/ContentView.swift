//
//  ContentView.swift
//  Shared
//
//  Created by Nicholas Orlowsky on 2/28/22.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        Button(action: {
                   poc()
               }, label: {
                   Text("Crash Kernel")
               })
        
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

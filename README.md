# tic_tac_toe_self_learning
A self learning tic tac toe program using FANN and C

The design is set up thusly to accommodate easy plugin of new versions
and includes a human interface to test skill against..

The design is a typical client server setup, where the server mandates
the rules, keeps score, assigns players etc. The clients battle each other, 
deploy intelligence and talk to the server using a predefined protocol during 
gameplay.

This repo will contain multiple versions of clients, including a self learning
entity, but also a simpler minimax implementation.

# Application de messagerie instantannée

## Utilisation sous Windows

Pour s'assurer du bon fonctionnement de l'application sous Windows, il
faut compiler le client ou le server dans Visual Studio 2017. Le serveur
devrait compiler sans problème en 32bit et 64bit. Par contre, le client
_doit être compiler en 32 bit_ (x86 Debug), car la librarie pdcurses
incluse est 32 bits.

## Utilisation sous Linux

Pour le serveur, il suffit d'exécuter :

'''
$ make && ./Build/server
'''

Pour le client, il faut la librarie ncurses. Il suffit d'exécuter :

'''
$ make && ./Build/client
'''

## Notes

Pour le client et le serveur, il est possible d'utiliser l'options
'--help' à la ligne de commande pour voir les options possibles.

Pour quitter l'application, il suffit d'écrire '/quit' en tant que 
message et appuyer une touche.

## Bugs

Sous le client Windows, la touche 'backspace' ne fonctionne pas très
bien en raison de quelques différences entre 'ncurses' et 'pdcurses'.


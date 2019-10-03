

def getPlayerNameURL(player_number):
    return "player"+str(player_number)+"@nchain.com"

def getPlayers(number_of_players):
    player_list=[]
    Ordinal = 1
    for player_number in range(1, number_of_players+1):
        player_list.append( (Ordinal, getPlayerNameURL(player_number)) )
        Ordinal = Ordinal + 1

    return player_list



if __name__ == '__main__':
    for player_url in getPlayers(10):
        print(player_url)


#Description

Use the IMDB files from ftp://ftp.fu-berlin.de/pub/misc/movies/database/ to build a SQLite database that supports the following applications (which you will also build).

+ tag movie_name: prints all of tag lines used for all of the movies with the given name. Tag lines are printed one per line and are prefixed with the year of the movie.

+ movies rating year: prints all of the movies with the given MPAA rating in the given year along with the reasons for the rating in alphabetic order.
 
+ actors movie_name: prints all of the actors in all of the movies with the given name. Output is sorted by year then actor name.

************************************************************************************************************************************************************
##README.txt

1. formatData.sh reformats data from the .list files into text files which are more easily parsed

2. Movie_Actors_Database/src/main.java parses the data from actors.txt and creates the database movie_actors.db

3. Movie_Tag_Database/src/main.java parses the data from movies.txt and creates the database movie_tags.db

4. MPAA_Rating_Database/src/main.java parses the data from mpaa.txt and creates the database mpaa_ratings.db

5. main.c when compiled with makefile into imdb-query is a client with which the queries can be executed as per the assignment

************************************************************************************************************************************************************
Sample run:
************************************************************************************************************************************************************

    $ make
        
        gcc main.c -Wall -std=c99 -I/opt/local/include -lm -lsqlite3 -L/opt/local/lib  -o imdb-query
        
    $ ./imdb-query 
    
        Enter ? for command menu
        > ?
        --Commands

            tag movie_name: prints all of tag lines used for all of the movies with the given name. Tag lines are printed one per line and are prefixed with the year of the movie.

            movies rating year: prints all of the movies with the given MPAA rating in the given year along with the reasons for the rating in alphabetic order.

            actors movie_name: prints all of the actors in all of the movies with the given name. Output is sorted by year then actor name.

        exit - quit and exit the program.


        Enter ? for command menu
        > tag Batman Returns
        2009: "Bad Movie Beatdown"
        2012: "Game Grumps"
        2014: "Just Us League"
        2010: "Mr. Mendo"s Hack Attack"
        2006: "Video Game Vault"

        Enter ? for command menu
        > movies PG-13 2009
        The City of Your Final Destination (PG-13) - a brief sexual situation with partial nudity
        Beyond a Reasonable Doubt (PG-13) - a sex scene, violence and brief strong language
        Dragon Hunter (PG-13) - action violence
        Fire from Below (PG-13) - action violence and peril
        Superman/Batman: Public Enemies (PG-13) - action violence throughout and a crude comment
        Dragonquest (PG-13) - action/adventure violence, brief sexuality and drug use
        Hulk Vs. (PG-13) - animated action violence (Thor)
        Amreeka (PG-13) - brief drug use involving teens, and some language
        Hurricane Season (PG-13) - brief sexual humor
        The Marc Pease Experience (PG-13) - brief sexual material
        Moonlight Serenade (PG-13) - brief sexuality and some thematic material
        After Last Season (PG-13) - brief strong language
        Invictus (PG-13) - brief strong language
        The September Issue (PG-13) - brief strong language
        New in Town (PG-13) - brief strong language (original rating)
        Julie & Julia (PG-13) - brief strong language and some sensuality
        Le concert (PG-13) - brief strong language and some sexual content
        That Evening Sun (PG-13) - brief strong language, some violence, sexual content and thematic elements
        Dance Flick (PG-13) - crude and sexual content throughout, and language
        Year One (PG-13) - crude and sexual content throughout, brief strong language and comic violence
        Fired Up! (PG-13) - crude and sexual content throughout, partial nudity, language and some teen partying
        Land of the Lost (PG-13) - crude and sexual content, and for language including a drug reference
        The Smell of Success (PG-13) - crude and sexual content, language and a drug situation
        I Love You, Beth Cooper (PG-13) - crude and sexual content, language, some teen drinking and drug references, and brief violence
        Without a Paddle: Nature"s Calling (PG-13) - crude sexual humor and language
        Entscheidung in den Wolken (PG-13) - disaster sequences and some disturbing images
        Knowing (PG-13) - disaster sequences, disturbing images and brief strong language
        The Cove (PG-13) - disturbing content
        U.N. Me (PG-13) - disturbing thematic material involving genocide and sexual abuses, and for violent images
        Karma Calling (PG-13) - drug content and sexual references
        Fast & Furious (PG-13) - for intense sequences of violence and action, some sexual content, language and drug references
        Under the Mountain (PG-13) - frightening action and peril, and for some sensuality
        The Hole (PG-13) - frightening images, violence and some language
        Hulk Vs. (PG-13) - intense bloody animated violence (Wolverine)
        2012 (PG-13) - intense disaster sequences and some language
        Avatar (PG-13) - intense epic battle sequences and warfare, sensuality, language and some smoking
        Avatar (PG-13) - intense epic battle sequences and warfare, sensuality, language and some smoking (special edition)
        Fighting (PG-13) - intense fight sequences, a sex scene and brief strong language
        X-Men Origins: Wolverine (PG-13) - intense sequences of action and violence, and some partial nudity
        Transformers: Revenge of the Fallen (PG-13) - intense sequences of sci-fi action violence, language, some crude and sexual material, and brief drug material
        Terminator Salvation (PG-13) - intense sequences of sci-fi violence and action, and language
        12 Rounds (PG-13) - intense sequences of violence and action
        Push (PG-13) - intense sequences of violence and action, brief strong language, smoking and a scene of teen drinking
        Sherlock Holmes (PG-13) - intense sequences of violence and action, some startling images and a scene of suggestive material
        The Unborn (PG-13) - intense sequences of violence and terror, disturbing images, thematic material and language including some sexual references
        Surrogates (PG-13) - intense sequences of violence, disturbing images, language, sexuality and a drug-related scene
        The Stepfather (PG-13) - intense sequences of violence, disturbing images, mature thematic material and brief sensuality
        The Yankles (PG-13) - language
        Duplicity (PG-13) - language and some sexual content
        The Invention of Lying (PG-13) - language including some sexual material and a drug reference
        Motherhood (PG-13) - language, sexual references and a brief drug comment
        17 Again (PG-13) - language, some sexual material and teen partying
        Zhang wu shuang (PG-13) - martial arts violence
        My Sister"s Keeper (PG-13) - mature thematic content, some disturbing images, sensuality, language and brief teen drinking
        To Save a Life (PG-13) - mature thematic elements involving teen suicide, teen drinking, some drug content, disturbing images and sexuality
        Like Dandelion Dust (PG-13) - mature thematic material including domestic violence and alcohol abuse
        I Can Do Bad All by Myself (PG-13) - mature thematic material involving a sexual assault on a minor, violence, drug references and smoking
        Life 101: Angel"s Secret (PG-13) - mature thematic material involving consequences of teenage sexuality and abortion
        The Lovely Bones (PG-13) - mature thematic material involving disturbing violent content and images, and some language
        An Education (PG-13) - mature thematic material involving sexual content, and for smoking
        Madea Goes to Jail (PG-13) - mature thematic material, drug content, some violence and sexual situations
        Greta (PG-13) - mature thematic material, some sexual content and drug references
        Cold Souls (PG-13) - nudity and brief strong language
        The Blind Side (PG-13) - one scene involving brief violence, drug and sexual references
        Fanboys (PG-13) - pervasive crude and sexual material, language and drug content
        Couples Retreat (PG-13) - ppeal for sexual content and language (re-rating on appeal)
        Rock Slyde (PG-13) - ppeal for sexual material including language
        The Deported (PG-13) - rude humor and language
        Star Trek (PG-13) - sci-fi action and violence, and brief sexual content
        Drag Me to Hell (PG-13) - sequences of horror violence, terror, disturbing images and language
        Cirque du Freak: The Vampire"s Assistant (PG-13) - sequences of intense supernatural violence and action, disturbing images, thematic elements and some language
        Echelon Conspiracy (PG-13) - sequences of intense violence and action, some sexuality and brief language
        Armored (PG-13) - sequences of intense violence, some disturbing images and brief strong language
        Green Lantern: First Flight (PG-13) - sequences of sci-fi action violence
        Fung wan II (PG-13) - sequences of stylized martial arts violence throughout
        Street Fighter: The Legend of Chun-Li (PG-13) - sequences of violence and martial arts action, and some sensuality
        Angels & Demons (PG-13) - sequences of violence, disturbing images and thematic material
        My Life in Ruins (PG-13) - sexual content
        He"s Just Not That Into You (PG-13) - sexual content and brief strong language
        Bran Nue Dae (PG-13) - sexual content and drug use
        Labor Pains (PG-13) - sexual content and language
        My Father"s Will (PG-13) - sexual content and language
        My One and Only (PG-13) - sexual content and language
        Play the Game (PG-13) - sexual content and language
        Coco avant Chanel (PG-13) - sexual content and smoking
        Nine (PG-13) - sexual content and smoking
        He Who Finds a Wife (PG-13) - sexual content and thematic elements
        Whip It (PG-13) - sexual content including crude dialogue, language and drug material
        All About Steve (PG-13) - sexual content including innuendos
        Ghosts of Girlfriends Past (PG-13) - sexual content throughout, some language and a drug reference
        Goal! III (PG-13) - sexual content, brief strong language and some drug use
        Stay Cool (PG-13) - sexual content, language including some drug references, and for some underage drinking
        The Proposal (PG-13) - sexual content, nudity and language
        The Six Wives of Henry Lefay (PG-13) - sexual content, nudity and some language
        Oy Vey! My Son Is Gay!! (PG-13) - sexual content, partial nudity, language and some thematic material
        Steppin: The Movie (PG-13) - sexual content, rude humor and language
        City Island (PG-13) - sexual content, smoking and language
        Nobody (PG-13) - sexual content, some drug material and language
        (500) Days of Summer (PG-13) - sexual material and language
        Obsessed (PG-13) - sexual material including some suggestive dialogue, some violence and thematic content
        The Strip (PG-13) - sexual references
        The Jerk Theory (PG-13) - sexual references and language
        Not Easily Broken (PG-13) - sexual references and thematic elements
        Post Grad (PG-13) - sexual situations and brief strong language
        Whatever Works (PG-13) - sexual situations including dialogue, brief nude images and thematic material
        Gentlemen Broncos (PG-13) - some crude humor
        Ice Twisters (PG-13) - some disaster images
        MegaFault (PG-13) - some disturbing disaster images (special edition)
        Polar Storm (PG-13) - some disturbing images
        The Greatest Song (PG-13) - some drug and sexual material
        Love Hurts (PG-13) - some drug content and language including sexual references
        The Haunting in Connecticut (PG-13) - some intense sequences of terror and disturbing images
        Creation (PG-13) - some intense thematic material
        Shadowheart (PG-13) - some intense western violence
        Flying By (PG-13) - some language
        Paper Heart (PG-13) - some language
        The Open Road (PG-13) - some language
        Bring It On: Fight to the Finish (PG-13) - some language and innuendo
        Not Since You (PG-13) - some language including brief sexual references
        Good Hair (PG-13) - some language including sex and drug references, and brief partial nudity
        Love Happens (PG-13) - some language including sexual references
        Chuyen tình xa xu (PG-13) - some sex-related content
        H2O Extreme (PG-13) - some sex-related content and language
        I Hate Valentine"s Day (PG-13) - some sexual content
        Reach for the Sky (PG-13) - some sexual content
        White Wedding (PG-13) - some sexual content
        The Break-Up Artist (PG-13) - some sexual content and language
        All"s Faire in Love (PG-13) - some sexual content including references
        The Boys Are Back (PG-13) - some sexual language and thematic elements
        Love N" Dancing (PG-13) - some sexual references
        Did You Hear About the Morgans? (PG-13) - some sexual references and momentary violence
        The Maiden Heist (PG-13) - some strong language, nudity and brief fantasy violence
        De indiaan (PG-13) - some thematic elements
        Put (PG-13) - some thematic elements, drug use and smoking
        The Winning Season (PG-13) - some thematic elements, language including some sexual references, alcohol abuse and smoking
        Get Low (PG-13) - some thematic material and brief violent content
        Dear Lemon Lima (PG-13) - some thematic material and language
        See What I"m Saying: The Deaf Entertainers Documentary (PG-13) - some thematic material and language including sexual references
        Poema de salvación (PG-13) - some thematic material and violent content
        God Send Me a Man (PG-13) - some thematic materials
        B-Girl (PG-13) - some violence
        Royal Kill (PG-13) - some violence
        Star Quest: The Odyssey (PG-13) - some violence and a brief sexual reference
        The Twilight Saga: New Moon (PG-13) - some violence and action
        Nowhere to Hide (PG-13) - some violence and disturbing images
        Oveja negra (PG-13) - some violence and sexual content
        State of Play (PG-13) - some violence, language including sexual references, and brief drug content
        Ondine (PG-13) - some violence, sensuality and brief strong language
        The Least Among You (PG-13) - some violence, thematic elements and brief drug material
        For Sale by Owner (PG-13) - some violent and disturbing images
        Citizen Jane (PG-13) - some violent content
        White on Rice (PG-13) - some violent images and sex-related humor
        Palo Pinto Gold (PG-13) - some western violence
        Angel and the Bad Man (PG-13) - some western violence and a brief sexual reference
        From Mexico with Love (PG-13) - sports violence, language, brief sensuality and drug references
        G.I. Joe: The Rise of Cobra (PG-13) - strong sequences of action violence and mayhem throughout
        Everybody"s Fine (PG-13) - thematic elements and brief strong language
        Irene in Time (PG-13) - thematic elements and some sexual content
        The Time Traveler"s Wife (PG-13) - thematic elements, brief disturbing images, nudity and sexuality
        American Primitive (PG-13) - thematic elements, sexual material and language
        The Soloist (PG-13) - thematic elements, some drug use and language
        The Box (PG-13) - thematic elements, some violence and disturbing images
        Black and Blue (PG-13) - thematic material including drug content and some domestic abuse, and for language
        The New Daughter (PG-13) - thematic material including violence, disturbing images and brief strong language
        Pastor Brown (PG-13) - thematic material involving sexuality, and some language
        Corky"s Hot Ice (PG-13) - thematic material involving sexuality, partial nudity and some crude humor
        Adam (PG-13) - thematic material, sexual content and language
        3 Idiots (PG-13) - thematic material, some disturbing images and crude humor
        All Hell Broke Loose (PG-13) - violence
        Chandni Chowk to China (PG-13) - violence and martial arts action
        9 (PG-13) - violence and scary images
        Fireball (PG-13) - violence and some disturbing images
        The Shortcut (PG-13) - violence and terror, language, sexual material, thematic elements and brief teen drinking
        Wonder Woman (PG-13) - violence throughout and some suggestive material
        Carriers (PG-13) - violence, disturbing content and language
        Endgame (PG-13) - violence/disturbing images and some strong language
        The Uninvited (PG-13) - violent and disturbing images, thematic material, sexual content, language and teen drinking
        The Imaginarium of Doctor Parnassus (PG-13) - violent images, some sensuality, language and smoking
        The Fourth Kind (PG-13) - violent/disturbing images, some terror, thematic elements and brief sexuality
        Mexican Gold (PG-13) - western violence
        Bounty (PG-13) - western violence and brief language

        Enter ? for command menu
        > actors Dirty Harry
        1971: Al Dunlap
        1971: Albert Popwell
        1971: Allen Seaman
        1971: Andrew (I) Robinson
        1971: Bill Couch
        1971: Bob (I) Harris
        1971: Charles (II) Murphy
        1971: Charles Washburn
        1971: Christopher Pray
        1971: Chuck (I) Hicks
        1971: Chuck Dorsett
        1971: Clint Eastwood
        1971: Craig (I) Kelly
        1971: David (I) Gilliam
        1971: Dean (II) Webber
        1971: Derek Jue
        1971: Don (I) Haggerty
        1971: Don Siegel
        1971: Eddie Garrett
        1971: Ernest (I) Robinson
        1971: Frederic Ross
        1971: George Burrafato
        1971: George Fargo
        1971: Harry Guardino
        1971: Jack (I) Hanson
        1971: James (I) Nolan
        1971: James W. Gavin
        1971: Joe (I) Finnegan
        1971: John (I) Tracy
        1971: John (I) Vernon
        1971: John Frederick Vick
        1971: John Garber
        1971: John Larch
        1971: John Mitchum
        1971: John W. Peebles
        1971: Josef Sommer
        1971: Kristoffer Tabori
        1971: Larry (I) Duran
        1971: Leslie Fong
        1971: Marc Hertsens
        1971: Maurice Argent
        1971: Max Gail
        1971: Raymond (I) Johnson
        1971: Reni Santoni
        1971: Richard (I) Lawson
        1971: Richard (II) Samuelson
        1971: Scott (I) Hale
        1971: Sean Maley
        1971: Stephen Zacks
        1971: Stu Klitsner
        1971: Tony Dario
        1971: Victor (I) Paul
        1971: Vince Deadrick Sr.
        1971: William (I) Paterson
        1971: Woodrow Parfrey
        2007: Clint Eastwood
        2007: Gene Hackman
        2007: Laurence Fishburne
        2016: Dinos Pontikopoulos
        2016: Yiannis Niarros

        Enter ? for command menu
        > exit
        
    $ 

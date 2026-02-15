#include "NpcStrings.h"

static std::vector<std::string> splitLinesNpc(const std::string &s) {
    std::vector<std::string> lines;
    std::stringstream ss(s);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> NpcStrings::NPC_QUOTE_STRINGS = splitLinesNpc(R"(Have you met Noah? Yeah, he's a... well, he's my brother!
How did you get back here?
Oh boy, I sure love biking! I hear there's a place called Cycling Road, I can't wait to go!
Hehe, I am the Chief of this town! Learn some respect sonny!
If there's one thing I learned from being a Rocket, it's to always respect the boss...
I'm the muscle of this organization, wanna tussle?
How did you get back here?
How did you get back here?
These fools won't feed me! I haven't eaten that much...
Ugh, I've been waiting for hours! What's taking them so long?
Did you know? I'm really small!
Rumor has it that a grampa like me has a move for you...
Oh? Yeah, I know you have a crush on me!
My brothers and I have lived here all of our lives. Maybe now I should go out and catch some pokemon!
Darnit, my elevator broke again! How can I get to the second floor now?
Buzz off, chump!
Chump off, buzz!
How did you get back here?
I sure do LOVE this hotel! Not that you could ever afford it.
Wow, that lady sure is a beauty! I wonder if I should talk to her...
Tea? No lad, but maybe you'd like some water. I hear you can get some at the department store!
You're working on your pokedex? Hey, me too!
You're taking on the league challenge? Hey, me too!
You're wondering why we're here? Hey, me too!
Leave me alone, I'm coding a game!
CHUMBA!
Alright, fine, you can have my Eevee... just treat it nicely, okay?
How did you get back here?
How did you get back here?
Hey, do you have any spare change? I spent all my money on balls!
My daddy loves me sooo much! He bought me, like, a billion balls!
How did you get back here?
I looove X items! You can really just trivialize any bettle, heh!
I might be young, but I keep getting money from beating my friends! I wonder what I should spend it on...
How did you get back here?
I sure love playing games! Maybe you should play one, too!
UGH, my eyes hurt, I need to take a break.
No, it broke free! I'd better reset.
Sigh, if only I could afford a game system, too!
Hoho, aren't you a feisty one!
Hey, like these? Yeah, those are MUSCLES!
How did you get back here?
How did you get back here?
How did you get back here?
G'day, mate!
I'm so beautiful! I can hardly stand it!!
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
My glasses look so cool!
EEK, stranger danger!
Yeah I live here, you got a problem with that?
Cerulean's waters are so calming... I could stand here all day.
Did you know the bridge used to be even longer?
I'm cataloging the patterns of the city layout!
The gym's lights can be seen from here at night.
I love the breeze coming off the water.
Hey squirt! This gym uses water-type pokemon, I hope you've got a grass-type!
Intruder alert!
Why yes, I DO come here often!
Ugh, my boyfriend is such a dork.
Intruder alert!
Misty suuure is pretty! Not that she'd ever like a guy like me.
Psst... can I set you up with my daughter?
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Gosh, this place is a mess! Want to go grab lunch?
Yo, this gym uses fire-types! I hope you've got a water-type!
Volcanoes make me nervous, but the science is fascinating.
Those machines must have cost a fortune!
Fossils are like messages from the past.
Be careful, some samples are fragile.
Nothing beats fishing by warm volcanic waters.
This device can bring Pokémon back from extinction.
Metronome is unpredictable, just like science itself.
Intruder alert!
I work at the Silph Company! Have you heard of it?
Nothing like grabbing a snack during your break... just don't tell my boss!
Intruder!
Oh man, I got burned pretty badly...
Well hello there! Aren't you lovely!
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Fair warning, we have a very eccentric daugther!
Are you here to babysit? Perfect, we can have a date night now!
GET OUT OF HERE!
I was robbed down there, I'd be careful!
The guy on the other side lies, don't listen to him!
Bill used to tinker with everything in this house.
Heh, Fuchsia's always good for a gamble.
I wonder if Bill's grandpa still remembers me.
The Safari Zone is just beyond these gates!
You can't win big without taking risks.
The fish bite better when it's foggy.
I'm training hard so I can go to the Safari Zone alone.
An ordinary Poké Ball lies here.
Seel is basking in the cool marsh air.
An old fossil is half-buried in the ground.
NO!
I feel so bad for those pokemon on display.
Have you been to the dungeon? You know the one!
NO!
Hey broseph, you should rock out with me!
I REALLY do not think you should be rocking out with strangers.
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Hello... poison, have you heard of it? PSYCHIC, that's what I'd recommend!
And then *I* was like-
What? He said THAT?!
Well isn't that something!
Try your luck, the odds favor the bold.
Please exchange your coins at the counter.
The slots were better back in my day.
A little risk makes life exciting.
The Game Corner hum never gets old.
I just came for the free snacks.
House rules are posted behind me.
If you need help, I'm your guide.
I've hit the jackpot once, just once.
Prizes this way, don't forget your tickets.
I've seen rookies win big in here.
Pick your prize carefully, trainer.
NO!
I've gotta be honest with you, I have absolutely no confidence in you.
CRUD, I lost again! Maybe I should catch new pokemon.
NO!
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Have you heard of Cubone? I've always wanted to meet one.
NO!
They didn't tell me my hair would start thinning... it's not too bad, is it?
It's days like these, maaan, let me tell you!
NO!
My mustache itches, can you scratch it?
My mom is soo mean, will you adopt me?
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Man, this is the worst vacation ever! I swear my room is haunted...
I'M A GHOST! Haha, just kidding... or am I?
Purple is my favorite color! What's yours?
I wonder where Mr. Fuji ran off to...
Honestly, good riddance to that old fool, he ALWAYS made me wash the dishes!
NO!
The Moon Mountain is apon us, stay close...
My friends call me the gentleman... what's your name?
My son speaks in riddles! How can I ever love him?
You really need to step away RIGHT NOW!
Dinosaurs once walked where we stand.
I lost all my money in the Game Corner again...
These fossils are older than anyone can imagine.
Every specimen here tells a story.
I want to be an archaeologist someday!
Oak used to bring me here when I was little.
The museum helps us understand Pokémon history.
This place feels a little spooky, doesn't it?
Rocks remember things people forget.
I wonder where Professor Oak is today.
Please don't touch the equipment.
These machines help analyze Pokémon data.
Nicknames, huh? Yeah, they're important alright!
Just a small town giiirl.
Living in a looonely woooorld!
Hey buster brown! This here is a rock-type gym, ROCK ON! I feel very bad for you if you have a Charmander.
NO!
I want to work here when I grow up! Although being unemployed is nice, too.
Quit bungling about, you're going to knock something over!
Pewter's gym leader is tougher than he looks.
The mountain air keeps you sharp.
I built my first Pokédex model right here in town.
Fossils from Mt. Moon are priceless.
I'm training so I can beat Brock someday.
My Nidoran is growing up so fast!
Brock used to play here when he was a kid.
MEOW!
I love trains! Do you know where I can catch one?
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Yeah, yeah, I'm a pretty smart dude! But can I ask you a question?
AHHHH! How was my scream? Pretty good, right?
Oh no, he's loose...
Okay I'll admit, cloning and gene tampering might not have been my best idea!
Hey, have you seen a cat-like pokemon? We may have... messed up...
Hi! Do you like my coat? It's comfy and easy to wear!
I've been in a lot of clubs in my life, but this one is my favorite!
Look at my Seel! Soo cute!
BUHH!
Hey now, don't interrupt the master! He's very busy.
BUH!
What? Don't call me that, it's rude!
YOU ARE SO BEAUTIFUL! I mean, hi!
The men in this town are just awful, they'll flirt with anybody!
I'm looking to expand my family, will you be my child?
Are you lost? You don't look like a scientist to me!
Anybody can be a scientist! Me? No, no, I'm a cosplayer.
MY SISTERS MUST BE POSSESSED! Can you take me away from here?
Hello my child. Come sit and enjoy the feast in your favorite chair, perhaps something good will happen!
NOO!
Yes? Yes!... no? Well okay.
Magikarp? Yes, that IS the strongest pokemon, how did you know?
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
Hey pal, wanna battle? No? Okay.
'Scuse me, have you seen Voltorb? That one's my favorite!
Rock Tunnel gets darker the deeper you go.
Only authorized trainers may pass.
I'm heading to Vermilion as soon as I can.
Please present your badge at the counter.
This gate connects two very different routes.
What do you think of these muscles? Heh.
I came here to watch the trainers go by.
I bet you've traveled far already!
I want to go on an adventure too.
Kanto is so much bigger than I thought.
Make sure you're ready before heading north.
I train here to build my endurance.
Did you see the Power Plant nearby?
Route 4 has some sneaky wild Pokémon.
Welcome aboard! Enjoy your trip.
Let me tell you, all the passengers are LOADED with cash! You? Hmm...
Yohoho and a bottle of yo! Ho?
The sea air agrees with me.
I've never been on a ship this big!
The captain loves his rare Pokémon.
I hope we don't hit any storms.
These cabins are surprisingly cozy.
Fishing from a moving ship is tricky.
I'm mapping the layout of the ship.
The view from here is incredible.
Make sure your footing is steady on deck.
We're prepping a feast for the voyage.
Keep the stoves at a steady flame.
Don't let the soup boil over!
Fresh ingredients make all the difference.
We never stop cooking down here.
Everything must be served piping hot.
The captain expects perfection.
I love exploring the tall grass without worry.
These Pokémon patterns are fascinating!
Rock music keeps me energized while walking.
Silph Corporation's tech makes the Safari safer.
I never knew there were so many rare species here.
Data collection is key to understanding wild Pokémon.
Be careful, some Pokémon are shy around humans.
Studying Pokémon behavior is so rewarding.
I've been training my party to react quickly.
Silph's research helps trainers like you.
Mister! This gym has psychic-types, so you'd better have... uhh, a bug or something?
Don't get in my way or you'll regret it.
Hehe, with Team Rocket in charge I can do anything!
Silph Corp keeps the city running smoothly.
Music makes even city life bearable.
Science always comes first, even here.
I've got work to do, no time for visitors.
I admire trainers who follow the rules.
This area is heavily secured - beware.
We don't tolerate troublemakers in Saffron.
Stay clear, or you might get caught.
NO!
Psst, I bet we could get away with ANYTHING in here! What do you say?
I just love spending time with my Pidgey.
I'm training hard to be a top trainer!
There's a note here... I wonder who left it.
NO!
Looking good today, trainer!
Heal often, it's free!
Oh, did you want to get through here? Yeah, you'd like that, wouldn't you?
NO!
Hey, what are you staring at?
Help, we're trapped!
I am soo scared.
Please, don't let them hurt us...  
We're trapped here... save us, trainer!  
I just wanted to finish my work...  
They've taken control of the building!  
Stay back! They've got Pokémon ready to fight!  
I can't move without risking my life...  
Hurry, the boss is dangerous!  
Don't let them take any more Pokémon!  
We're counting on you to stop them...  
I hope someone comes before it's too late...
Darn, if only I had my equipment! I could fix you up in a jiffy.
LEAVE ME TO MY CHAMBERS!
I'm lost, can you help me find the bathroom?
Crazy how the tunnels connect, huh? I heard Digletts helped with construction!
Oddish, yes! That's right!
I walk these routes sometimes. If you see me, say hello!
I've heard that some pokemon evolve through trading... I wonder how you could do that!
I just honestly... yeah. You know?
This city is so beautiful! You should watch the sea at night.
Sailing is my hobby, but I lost all my pokemon! Can you help me find them?
Oh, oh, OH! Boss, this is an electric-type gym! Go catch a Diglett NOW!
NO!
Sometimes I wonder... how can the clerk hear us from behind the counter?
Oh no, you've got a crush on me, don't you? I'm only gonna break your heart!
NO!
Tales of a far-away region have blessed my ears... Johto! I'd love to visit some day.
I've been living here for days now, I wonder if anybody's noticed.
Huh, just a piece of paper? Useless.
You're from Pallet Town? I love visting there!
I might be young, but I know a strong trainer when I see one! I'll tell you if any pass by.
Did you enjoy the woods? Personally, I'm always a little scared in there.
On your way north, eh? That's actually my favorite direction!!
My sister and I hangout here for fun. Do you have friends?
Hi, you look friendly! Now please leave.
Birds are so pretty! I hate bugs, though.
I'm looking for a girlfriend. She should be around here somewhere!
Gaming, yes! That's what life is all about.
I don't know this man, can you rescue me?
NO!
Hey trainer, you look new to shopping! Pro-tip: buy repels... oh wait.
You must be new in town, I'm the city champion! Oh yeah, no trainers stronger than me here...
You finally made it! Ground-types rule here, how about you get them WET?!
NO!
Hiya sonny, are you catching lots of pokemon? Jay will be so proud.
I've heard a super strong trainer lives around here. I wonder where, though...
Howdy, you must be new here! Don't talk to me, ever.
...are you sure? You are so weak. NOBODY can SAVE you down there.
Yum yum, I'm the breeder! I'll borrow your pokemon to make more beautiful children with... just give me a second......
Feeling trapped? Perhaps these lonely statues want to be talked to... make sure to look them in the eyes!
Yes, I AM dancing because Matt is so cool!...Me? No, I'm not Matt in disguise! I just think I'm... I mean, HE'S very cool!)");

std::vector<std::string> NpcStrings::TRAINER_SPEECH_STRINGS = splitLinesNpc(R"(Hey, I was trying to catch pokemon!;Ahh okay, I guess you can hang!
Whoa, nice shoes!;No need to be that way...
Is it just me, or is it HOT?;Ice cold baby...
I hear there's a secret cave somewhere.;Wait, what was that?
Eek, get away from me!;Meanie!
Yikes, your pokemon look strong!;Golly, I was right!
Any luck finding pokemon?;Wow, you've got some strong ones.
Huh, what's that smell?;Ahh, the smell of defeat...
What, you live here, too?;But this was my home...
Any chance you'll go easy on me?;Yappers, you're a feisty one!
Crikey, we've got a live one!;FIRE!
What's that, burning PokeBread?;Woops.
Tiddle dee, tiddle doo -;A scoop of PokeSauce for you!
Hah, you look weak!;NO! This can't be!
Have you met my pokemon?;Well aren't you rude!
I lost my Pikachu!;I am nothing without my rat.
Charmander is a hot one, hot hot HOT!;Hm?
Did you drop these?;Oh, those are my pokeballs!
Potions, huh? Yeah, those are useful... oh, a battle?;I like repels!
I've got a present for you...;What? Where'd my wallet go?
I can name an orange pokemon!;Let's seee here...
Aren't you a snarkster!;Okay, I give up!!
Ouch! Let's battle cutie!;OUCH! NOT LIKE THAT!
Yo yo, looping like a yo-yo...;Wind and wind (hey hey whoa hey).
Winter is NOT my favorite season... sorry, one second.;But yeah, it's like he doesn't even love me...
Bulbasaur? Hah, I wish!;Maybe I'll catch a Pidgey?
I've heard rumors about other regions...;I think it was... the CINNABAR Region! Wait...
So there I was, just standing there.;AHHH!
Double or nothing!;Nothing...
I have three siblings! And you?;OH BROTHER!
My boyfriend is gonna hear about this!;Dumped!
My girlfriend completed her pokedex.;Wait a seo, who are you?
10,000 BOLTS!;CHAAAAARGE!
My perfume is brand new, how about that?;What a smelly day...
Hungryyy...;Thirsty!
Yeek my skirt!;Haha, just the wind I suppose...
I love to SURF!;Surf's up!
What are you staring at?;HMPH!
I cosplay as my favorite pokemon on the weekend - Vaporeon!;And here I thought I was a water-type trainer...
WAHHH!;Sorry, just a little cry!
Which pokemon would you be?;I'm the sad-type pokemon...
Hot dog, aren't you a rocket!;BLAST OFF!
Meowth is soo cute, do you have one?;I guess it's pay day!
ZAP! I've trapped you in my battle.;NO I'M TRAPPED!
Any luck recently?;Oh you know.
You know, standing on people is rude.;Kyaa!
Let's go pal!;BUDDY! Not good...
I wish my beanie was here...;I should call him.
Ohh my precious pokemon!;I am so lonely.
Hey bubba, got the news?;Wowza!
You picked the wrong patch of grass!;Guess the grass wasn't greener...
My Pokémon are itching for a fight!;Oof... they were itchy alright.
Careful! I just learned a new move!;...I should've read the description.
Don't blink or you'll miss it!;I blinked.
I train every single day!;Maybe I should take Sundays off...
You can't beat strategy!;So that was brute force, huh?
My Pokémon trust me completely!;I let them down...
I've got type advantage!;Wait-what type was that?
This battle is already decided!;I decided wrong.
I woke up early for this!;Should've stayed in bed.
No mercy, kid!;Okay, a little mercy...
You stepped into my training route!;I'm stepping out of it now.
I can smell a victory!;That wasn't victory...
My Pokémon love to show off!;They're shy now.
This is my lucky day!;Luck has left the building.
You won't get past me!;Past me you go...
I read all about battling!;Books lied to me.
Time to test my team!;Test failed.
I'm feeling confident today!;Confidence shattered.
I'm undefeated on this route!;...Was.
Let's see what you're made of!;Apparently, too much.
You look unprepared!;I was the unprepared one.
I've been waiting for a challenger!;I'll stop waiting now.
This'll be quick!;Too quick...
I won't hold back!;I should've held back.
My Pokémon and I are in sync!;Out of sync now...
You don't stand a chance!;Turns out, I didn't.
This is where your journey ends!;This is where my nap begins.
I've got surprises in store!;Surprise! I lost.
Let's make this exciting!;That was... excitingly painful.
Huh, what's with this weather?;Dang sun is in my eyes!
Battling? It's always a good time!;MeeeOWch...
A couple of guys, up to no good!;Training up in your neighborhood!
My brother is the best trainer! He can beat a twerp like you!!;Brooo...
Did you know? I walk this way every day!;Sketchy bridge!
I'm learning Pokenese!;Guess I better hit the books.
Tag, you're it!;I guess this is it for me.
Wonderful, a weakling!;Oh, wonder...
Who? What? When? Where?;Why? HOW?!
I've got a smart Butterfree! Wait, let me check my balls.;Huh?No dice!
I heard repels keep pokemon away!;I've been repelled!
Pika pika?;Well...
A joust, good sir!;Engarde!
I like to watch people battle.;Back to the stands with me!
Hi!;Hello?
No, wait a sec!;Beaten like a bug!
Bullseye! A trainer!;Blast nabit, I've been dooped.
Ahh, how I'd love to soar through the air!;No, I'm floating away!
Like my socks?;Yeah, thought so...
Hi, what's your name?;...Okay then.
Hey, can you help me out with something?;I can't stop giving kids money!
Mom gave me a big allowance this week!;Down the drain it goes...
I heard there were thieves around here.;You cheat!
What's your favorite pokemon?;I love Shellder! He's got quite the tongue...
Can I measure your friends?;Big and bold!
One day I'll win!;But not today...
I've got a tough team!;Tough luck!
My friends all listen to music.;I hear nothing but silence.
I love to sing!;I think I'll take up skydiving.
Sports, yeah!;A sportster like you doesn't play!
Play with me!;Hehe, yeah that was fun!
I love you!!;I hate you.
I heard about a secret pokemon with cat-like features.;But you probably don't have enough monsters.
They say a reclusive man lives in the west.;He seems to hold secrets from another world.
I heard somebody got lost on their way to the professor.;...probably just a rumor, right?
Hey, aren't you that guy?;Nope, sorry.
You'll BLACK OUT when I'm through with you!;I'm through!
My wife's daughter is a great trainer!;I should call her.
Wowza, WOW-ZA! I can't battle you!;See?
It's my birthday tomorrow!;Actually, that was yesterday.
A wild Magikarp appeared!;It's super effective!
I'm looking for friends!;Where are they.
Scott, is that you?;Wait, sorry, I meant Matt!
I think I just fell for you.;Aren't you lovely!
HEY, paws to yourself man!;Oh, that was me!
My heart cries out in sorrow.;Well I'll be!
Soo, wanna catch a movie?;reJECted!
Hahaha, fool!;No, not like this!
Can I have a hug?;I'm in a bind!
My dad told me about trainers like you!;Not a nice bone in your body, huh?
Hey! Eyes up here!;Guess I blinked...
I just polished my Pokéballs!;Scuffed already...
Careful where you step!;I warned myself too late.
Training builds character!;So does losing, I guess...
I woke up with battle energy!;I need a nap...
My Pokémon are fired up!;Someone get the extinguisher...
You ever just feel a battle coming?;Yeah... I felt that.
Let's make this interesting!;Interesting hurts.
I memorized all the type charts!;Forgot that one...
I skipped lunch for this!;Bad decision.
You look lost!;Now I really am...
No distractions, let's go!;I got distracted.
I practiced this in my head!;Reality disagreed.
This route belongs to me!;I'll share...
I've got a trick up my sleeve!;Wrong sleeve!
Don't underestimate me!;Lesson learned.
I trained in the rain!;Still slippery...
My Pokémon never miss!;They missed.
I've been saving my strength!;Saving failed.
You won't forget this battle!;I want to forget.
I'm in the zone!;Evicted from the zone.
I came prepared!;Prepared... poorly.
I trust my instincts!;My instincts betrayed me.
This is my moment!;Moment passed.
Watch and learn!;I watched...
You're about to see something special!;That was... something.
No holding back now!;I should've held back.
I studied all night!;Should've slept.
My team is perfectly balanced!;They tipped over.
I feel lucky today!;Luck called in sick.
This is going exactly as planned!;Plan B?
I won't lose focus!;Lost it.
I've beaten tougher trainers than you!;Apparently not.
Let's make this quick and clean!;Messy and slow...
I'm on a winning streak!;Streak broken.
You're standing in my way!;I stepped aside.
My Pokémon believe in me!;They still do, right?
This battle decides everything!;Everything decided...
I've got momentum!;It rolled away.
No fear, no doubt!;Mostly doubt now.
I was born ready!;Born mistaken.
You picked the wrong trainer!;I picked the wrong fight.
I've got nothing to lose!;Lost anyway.
Let's do this, partner!;Sorry, partner...
I can feel the power!;It hurts...
This is my specialty!;Time to respec.
You're not getting past me!;You got past me.
I trained specifically for this!;Specifically failed.
I've waited all day!;Worth it? No.
I'm giving it my all!;That was all?
I'm unstoppable!;Stopped.
You'll regret challenging me!;I regret it.
I can still turn this around!;Nope.
This is my comeback!;False alarm.
You know, I've got a card collection, too!;Ack, you're no fun!
I've been watching you for a while now...;Peek-a-boo!
I'm gonna scream!;ah.
Get a mouthful of this!;I'm busted.
I'll be a gym leader some day!;But you're in the lead!
Hey, this one's mine!;Alright, you can have some berries.
Attack, my swarm!;Could've sworn that would work...
I saw a guy walk through that wall!;Eh, probably just my imagination.
The forbidden one shall strike soon.;What, you don't believe me?
Jackpot! Ding ding ding!;Ding-a-ling...
FLASH! Heh, surprised?;Hey, no pictures!
My son loves pokemon!;Donald?
Do you come here often?;I'm going!
I love sushi, it's my favorite!;Wait, it's made from WHAT?!
Care for a pokeblock?;Nah, you can't use these!
Ho-ho-ho!;Ho-oh?
I've trained for this exact moment!;Moment over.
My strategy is flawless!;Flawed.
Witness my true power!;I'd rather not.
This fight decides my fate!;Fate decided.
I won't go down easy!;Went down hard.
I sharpened my skills today!;Blunted.
My Pokémon are pumped!;Deflated.
You activated my trap card!;Wrong game!
This terrain favors me!;The terrain disagrees.
I calculated every move!;Bad math.
This is my battle theme!;It's off-key.
I practiced my victory pose!;Abort pose!
I can see the outcome already!;I misread it.
This'll be legendary!;Legendarily bad.
I won't forget your face!;I already forgot.
I'm feeling spicy today!;Too spicy.
I'm running on pure adrenaline!;And panic.
This route is my stage!;Curtain call.
I've got backup plans!;None worked.
You're about to learn something!;I learned instead.
My Pokémon trust me!;I hope...
I studied the meta!;Outdated.
This battle is personal!;It wasn't.
I brought my A-team!;Left them at home.
I've waited my whole life for this!;That's rough.
No one beats me twice!;Once was enough.
I've got one more trick!;That was the trick?
I'm not nervous, you are!;I am nervous.
I can still win this!;Statistically unlikely.
This ends now!;It ended.
I trained at high altitude!;I feel dizzy.
My instincts never fail!;They failed.
This is where legends are made!;Of caution.
I'm in perfect form!;Out of shape.
My Pokémon are synchronized!;Desynced.
I came here to win!;I'm leaving.
You won't break my focus!;Broken.
This is peak performance!;Peak passed.
I won't let you win!;I couldn't stop you.
Everything's going according to plan!;What plan?
I'm giving it 110%!;Still not enough.
I've got momentum!;Lost it.
I won't back down!;I'm backing down.
This is just a warm-up!;Too warm.
Brock is my hero!;Help me!
Buzz off, I'm busy!;Bzz...
Misty sure is a beauty!;What beautiful form...
Ten-hut! Private Johnson reporting for battle!;It's okay, Lt. Surge will exact my revenge!
Erika is my waifu! See? I've got her picture on my phone!;Wait, this is just a pokedex!
I once saw Koga hopping between roofs in the dead of night!;Huh? Just a shadow?
Sabrina... I mean what more can I say?;Gack! Psychic damage!
Blaine, that old fool...;He locked himself away years ago!
I bet you work for Team Rocket!;Don't you know?
Yes! I'm on world-record pace!;Noo, that's a time-loss...
This route is my training ground!;Evicted.
I studied Brock's every move!;I missed something.
Water types are unbeatable!;I can't swim...
Electric power, full charge!;Power outage!
Grass Pokémon are elegant!;Trampled...
Poison is an art form!;I poisoned myself.
Psychic power is overwhelming!;My head hurts...
Fire burns away weakness!;I'm burned out.
I live for the Gym Challenge!;I should've stretched.
One day I'll stand on that podium!;I slipped.
I've got Gym Badges-well, posters.;They fell off...
My idol would never lose!;...Right?
I heard the Gym Leaders talk about me!;Probably not.
This battle will be historic!;Historically brief.
I model my style after a champion!;Bad fit.
I memorized all the Gym puzzles!;Wrong gym.
I trained where legends trained!;I trained near it.
I've got leader-level confidence!;Rookie mistake.
They say power flows through me!;It leaked out.
I've been scouted by the League!;By a bird...
This match is regulation!;I forgot the rules.
I'm on Gym Leader hours!;Closed.
I can already hear the applause!;That's laughter...
I've faced tougher conditions!;Like what?
I've got the badge spirit!;Spirit crushed.
This arena feels familiar!;It shouldn't.
I'm destined for greatness!;Detoured.
I follow the Gym Code!;I skimmed it.
No Gym tricks here!;Just panic.
My strategy is Gym-certified!;Expired.
The Leader would approve!;They would not.
I trained with a former champ!;Briefly.
This is my qualifying match!;Disqualified.
I wear this uniform with pride!;It's backwards.
I'm ready for the spotlight!;Turn it off.
I'll earn my badge today!;Different badge.
I've watched every Gym battle!;From behind a rope.
I carry the legacy forward!;I dropped it.
This is my Gym Trial!;Trial failed.
I'm calm, cool, collected!;Panicking.
My resolve is unbreakable!;Cracked.
Congratulations, I'm your final challenge!;Haha, just kidding... unless...
I've got quintuple the firepower!;FIRE FIRE BURRRRRN!
Spare some change?;I need a new hobby.
Ready? Set?;Stop!
I am the queen of this cave, prepare to lose.;NO! I should have evolved...
Huh, you've made it pretty far! See ya at the Pokemon Center!;I think I need to heal.
Well well well, aren't you dazzling!;HAH! A beautiful foe, thank you for the hug darling!
It's getting hot in here...;Yee-ouch!
Is that burning toast?;Oh no...
Well, aren't you hot stuff!;Too hot!
I just LOVE cute trainers like you!;...you don't love me, do you?
Boy, I sure love Meowth!;I thought I could fool you...
You picked the wrong trainer, bud.;Hehe, it takes real skill to beat a PokeBurglar!
I'm the last line of defense!;Or at least, I was.)");

std::vector<std::string> NpcStrings::LEADER_SPEECH_STRINGS = splitLinesNpc(R"(Sigh... yep, I'm Brock. Let's do this.;Whoa, that was rocking! You earned yourself a badge and a TM, bide!
Hiya! Ready to get trounced by a cutie in a swimsuit-y?;Yikes, you're the real deal! Take a badge and teach your friends bubble beam!
Ready for a zapping, kiddo?;Well darn tooting, you're pretty good! Take this badge and SHOCK your opponents with thunderbolt!
Hmm? A battle? Well aren't you a strange little one.;Yawn, you're not half-bad. Here's a badge, go drain some health mega man!
俺様に勝つって？馬鹿に言うな、テメー！BATORU TAIMU!;アララ、負けちゃったって？まぁいーや、バッジをあげるわ！DOKU DOKU TAIMU!
Ufufu, I saw your arrival years ago. Let's-a-go!;Well shucks, my psywaves are acting up! Wear this badge man!
Please save me, I've been trapped in here for years... a battle? Hmph, you darn kids!;BLAST! That was firey, broseph! Take this gnarly badge!
At last, you've found me. Well play time is over, it's time to bring my squad back to life!;Ugh, I've gotten weak... but that was well done! You'll want to fissure the Elite Four... as for me? Goodbye.
You came to see me, eh? Time to die.;Huh? Well, I guess you're cheating anyway, go big or go home!)");

std::vector<std::string> NpcStrings::GIO_RIVAL_SPEECH_STRINGS = splitLinesNpc(R"(Another kid in my lair? You'll be lucky to escape alive!;Blast it... those idiots better be working hard in the tower!
Ohoho, it's you, huh? You've been quite the headache, prepare to be schooled!;Curses, foiled again! I have been bested, but I shall return again soon!
Hey buddy, my name is Noah! My grandpa hates me and won't give me a pokemon, but check out this cool one I caught!;Whoa buddy, you're pretty strong!
Hey buddy! I've gotten a bit stronger, wanna see?;Oh buddy, not good... I wonder if Brock would train me?
Buddy! I got some training from Brock, ready to see my new moves?;That's garbage buddy! Maybe Misty can help me out...
Well buddy, that old captain is a masoneer! But hey, Misty gave me some pointers, let's go!;Not good buddy... I heard there's a spooky cave nearby, good thing I can light my way through!
Buddy, you made it! This tower is very spooky, but Lt. Surge trained me to be strong!;Not strong enough... Anyway, rumor has it the top floor is the scariest of them all! Watch out.
Hey buddy, you're here too? Listen, there's a big scary man at the top, and I'm coming for him!;Shoot, even Erika couldn't prepare me... you know what? I just need every badge!
Buddy, you got every badge? Heh, I did, too! Let's see what you've got!;Still not enough, eh? I heard a man in Viridian City trains in isolation, maybe he'll train me...)");

std::vector<std::string> NpcStrings::E4_SPEECH_STRINGS = splitLinesNpc(R"(Hah, so the gym leaders HAVE gotten weaker! Well this should be interesting, I guess I can use you as warm-up training!;WHAT?! I lost?! Dammit! Alright, go on through, but don't expect to get past the next trainers!
Broo do you lift? I can bench 300, but I think I'm gonna hit a PR this week... oh, a battle? Sure bro, but my pokemon are pretty swole!;Crap, where's my creatine? You're pretty strong, but your calves could use some work... Anyway! Go on through, I believe in you!
OoOoOh, are you scared? No? Well you should be, I'm 500 years old and have never met a stronger trainer!;Okayy, maybe there was one, but I doubt you could beat him, HAH! Go through, squirt!
Eh-hem! Yes, hello, I am the most elite of the four! My dragons are quite terrifying - I'm surprised you lived through the last three battles, but that ends NOW!;Whoa, you're the real deal! I gotta hand it to you, not many can beat my devilish dragons! Anyway, you did great, you should go home now!... please, for your sake.
Heh... surprised? Yeah buddy, it's me, Noah. I won't hold back anymore - you're going to lose.;WHAT? BUT, but... ugh, fine, you're the champion, okay buddy? Evenso, you don't stand a chance against Jay!
So you beat Noah... I don't think I can vanquish this abomination by myself, but maybe you can help... Let's see what you've got!;Huh, you're not bad, kid. Maybe you've got what it takes to conquer the void!)");

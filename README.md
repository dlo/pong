Pong
====

My first game ever.

Building
--------

Before you compile, make sure you have the GLEW libraries. On Mac OS X:

    brew install glew

After you've installed them, just run

    make pong

Then, `./pong`, and behold:

![Video](https://raw.github.com/dlo/pong/master/video.gif)

Platforms
---------

* Windows (props to [@code_head](http://twitter.com/code_head) for this)
* OS X

Gameplay
--------

Top player moves left / right with the 'a' and 'd' keys, respectively, while the bottom player moves with the arrow keys. If the ball breaks through your paddle, the other player gets a point. The game ends whenever you want it to end :).

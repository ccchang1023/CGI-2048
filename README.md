# CGI-2048

CGI-2048 is an AI program for Game 2048. This version is exactly the one used to participate in Taiwan 2048-Bot Tournament, http://2048-botcontest.twbbs.org/. The version was ranked the second in the tournament, as shown in http://2048-botcontest.twbbs.org/download/stats_2048.htm.

Our algorithm is based on the following two papers:

[1] Szubert, M. amd Jaskowaski, W., “Temporal Difference Learning of N-tuple Networks for the Game 2048”, IEEE CIG 2014 conference, August 2014.

[2] I-Chen Wu, Kun-Hao Yeh, Chao-Chin Liang, Chia-Chuan Chang, and Han Chiang, "Multi-Stage Temporal Difference Learning for 2048", the 2014 Conference on Technologies and Applications of Artificial Intelligence (TAAI 2014), Taipei, Taiwan, Novemeber 2014.

Since we don't have much time to refactor the code and write up documents, we left it as the way it is (could be hard to read). To run the program, need our trained data which can be downloaded from http://aigames.nctu.edu.tw/~cgilab/download/. The code for TD learning is not included, since the performance varies highly depending on the adjustment of several parameters, such as the training times, anyway.

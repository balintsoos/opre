1. Child kuldjon TERM signal-t parent-nek, az kezelje le, es kuldjon a child-nak egy TERM signalt! A child-ban legyen egy sleep(100) hivas a signal kuldes utan.
2. Ugyanez sigaction hasznalataval.
3. Harom masodpercenkent kuldjunk egy szamot signal-lal, timer hasznalataval, ami egyre novekszik! (Eloszor kuldjunk 1-est, majd 2-est, es igy tovabb.) Osszesen 5 uzenetet kuldjunk.
4. Letesitsunk egy pipe-ot parent es child kozott! Child kuldjon egy TERM, majd egy SIGUSR1 signal-t parentnek, parent irja be a pipe-ra, hogy milyen signal-t kapott! (Hasznalhatunk globalis valtozokat.)

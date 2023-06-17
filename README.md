# Multi-Threaded-Webcrawler

Program that spawns the user-specified number of threads to retrieve a PNG image from the specified web server using the libcurl library and saves it in `all.png`.

Usage: `./paster [-t <NUMBER OF THREADS>] [-n <WEB SERVER>]`.

Each time the webserver receives a request from cURL, it sleeps for an unspecified amount of time, blocking the current thread, and then sends a random horizontal strip of the PNG image. Each thread will continuously request PNG image fragments from the webserver and use those to recreate the PNG until all fragments have been received.

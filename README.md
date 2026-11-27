###                        ___ _        _    ____   ___ _____ 
###                       |_ _| |      / \  | __ ) / _ \_   _|
###                        | || |     / _ \ |  _ \| | | || |  
###                        | || |___ / ___ \| |_) | |_| || |  
###                       |___|_____/_/   \_\____/ \___/ |_|  
###                    A Faster Video Object Detection Algorithm

ILABOT (I Looked A Bunch Of Times) aims to be a video object detection algorithm specifically designed for projects requiring high framerate cameras, such as photography, highway speeding cameras, or any project where accuracy is just as important as seeing exactly when a fast-moving object enters and exits the frame. The project is a very early work in progress, and is not complete yet.

The project is mainly written in **C** and uses placeholders, such as **current.bmp** and **prev.bmp**, rather than actual video files. The project will not work if the directory that **main** is in does not have **current.bmp** and **prev.bmp**. I do not yet have a full build available, so to run, you must download the code and compile it yourself. A full build is coming in the future.

[!["screenshot of my work-in-progress code"]("https://private-user-images.githubusercontent.com/138725711/721730441-5470d2e2-4a80-4a85-a101-e3697e7908e0.png?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3OTU4MDczNTMsIm5iZiI6MTc5NTgwNzA1MywicGF0aCI6Ii8xMzg3MjU3MTEvNzIxNzMwNDQxLTU0NzBkMmUyLTRhODAtNGE4NS1hMTAxLWUzNjk3ZTc5MDhlMC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjYxMTI3JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI2MTEyN1QxOTE3MzNaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1lZjE3M2ExYmYxNDBkY2VlOTY0YzYzZDE4MzJiNWZmYjVkOTg3ZGMzNTE5MmI1NmEzY2QwZDQyZWRmNDM0ODY4JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZyZXNwb25zZS1jb250ZW50LXR5cGU9aW1hZ2UlMkZwbmcifQ.6QCjklniBxc1MEs3UBlIuRYjn8pnAm-gQlLAZ8QWlXw")]("https://github.com/ThumbsUpper/ILABOT/blob/faster/main.c")
*work-in-progress code*

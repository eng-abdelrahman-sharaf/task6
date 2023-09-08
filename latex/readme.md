# latex project
## be careful⚠️⚠️
please download the pdf and open it on  adobe acrobat to be able to see animation

## python script used to name png images  🐍🐍
png images were derived from the main gif image to get it frame by frame using
[Online Converter - Convert Image, Video, Audio, & Document Files (freeconvert.com)](https://www.freeconvert.com/pt/gif-to-png/download)

    # to add gif you should convert it to png files first
    
    """ if i want to add a string to the name of each file which extension is .png in direcrtory 1 """
    
      
    
    import  os
    
      
    
    # Define the path of directory 1
    
    dir_1  =  "C:/Users/ADMIN/Downloads/Compressed/AstarExampleEn"
    
      
    
    # Define the string to be added
    
    string  =  "soo-"
    
      
    
    # Loop through the files in directory 1
    
    for  file  in  os.listdir(dir_1):
    
    # Get the file name and extension
    
    name, ext  =  os.path.splitext(file)
    
    num  =  name.split("-")[-1] #name = the number of the frame
    
      
    
    # Check if the extension is .png
    
    if  ext  ==  ".png":
    
    # Create the new file name with the string added
    
    # "boo-ddd.png" ddd is the number of the frame
    
    new_name  =  string  +  num  +  ext
    
    # Rename the file
    
    os.rename(os.path.join(dir_1, file), os.path.join(dir_1, new_name))

##  how animation was animation added
using package "graphicx"

    \usepackage{graphicx}

then using a video generating (from the png files)function that takes these configs :

    animategraphics[loop,width=10cm,autoplay]{5}{DijkstraDemo/boo-}{0}{126}

 - loop (gif image doesn't end)
 - width (the width of the video)
 - autoplay (the video will start automatically)
 - {5} (frame rate = 5 frames per second)
 - {DijkstraDemo/boo-}{0}{126} png pattern which means that every png file is in the directory `DijkstraDemo` and has the name of "boo-`number`" as `number` is from `0 to 126`    



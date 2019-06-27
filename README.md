Cloudviewer
===========

## Building And Running
To build the project, run CMake to generate a file for your favorite build system.

### Linux / OSX
```console
$ cmake -H. -Bbuild
...
$ make -C build
```

This will produce the executable `./build/src/cloudviewer`.


### Windows
```console
$ cmake -H. -Bbuild -G "Visual Studio 15 2017"
```

This will produce a Visual Studio 2017 solution file `build/cloudviewer.sln`, that you can open and build - it will produce an executable like `cloudviewer.exe`.

## Requirements

You must use Git to track your progress. Please use meaningful commit messages - there should be several commits in your submission!

To submit your solution, create a `.zip` of your final result - it should include the `.git/` directory.

## Task Description
In this task, you will work directly with some 3D data that we have captured.

Our 3D camera captures point clouds 30 times a second. This data is stored in a proprietary
format we call "DePth Video" with extension `.dpv`.

Included here is an example file, `recordings/1.dpv`.

You have been provided code that can read the recording, one frame at a time, and a viewer
that can view a point cloud.

### Task 1: Playback
Your task is to use the ImGui UI library (included, check for docs [here](https://github.com/ocornut/imgui/wiki)) to create a Play/Pause button,
that will update the visible cloud according to the recording.

You can open up our DPV files using `std::ifstream`. The `.dpv` file format has a header, so you will
need to skip it before reading frames using `dpv_skip_header()` (see `DepthFrame.hpp`). Then, you can
read frames from the file into a `DepthFrame` using `dpv_read_depth_frame()`.

Notice that each `DepthFrame` has a timestamp - this is the time when the frame was first recorded.

For full credit, your playback must play the recording back at 1x speed - but notice that
frames don't occur at regular intervals - sometimes there is 68ms, sometimes 180ms between consecutive frames,
so you will have to take this into account in your playback, by making sure that the screen shows the same
delays that are in the file (so the frame should be visible for around 180ms before the next frame is shown).

Feel free to add other features like a current elapsed time indicator in the window, a color picker, or others.

### Task 2: Cleanup
Your next task is to isolate the user's points in the point cloud - by removing
the wall and the ground on each frame.

Your technique should be as efficient as possible. You can do this by filtering out points in the cloud itself,
or you are welcome to modify `SimplePclConverter::buffer_to_cloud()`
method in `nimbus/SimplePclConverter.hpp` to perform this more efficiently.


### Extra Task: Bar Identification
Now, use some technique to identify the location of the barbell that is being held by the user
in each frame. Create another `PointCloud` that is red, and use it to display only points that
belong to the barbell in each frame.

Hint: look at [RANSAC](https://en.wikipedia.org/wiki/Random_sample_consensus) - you might be able to
come up with a model for a barbell (a cylinder with diameter 28.5mm in the middle, 43mm at the ends), and check
for the  position. Notice also that the bar is always roughly horizontal.

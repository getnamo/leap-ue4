leap-ue4
====================
# v3 Plugin

Completely rewritten for modern tracking performance using v4 Leap tracking, the new plugin is available at 

https://github.com/leapmotion/leapunreal

with example project content at

https://github.com/leapmotion/leapunrealmodules

I highly recommend you use the new v3 plugin going forward!

# v2 Plugin

This plugin is considered deprecated, but remains available for older engine compatibility.

This repository concerns the v2 Leap Motion plugin, an event-driven [Leap Motion](http://www.leapmotion.com) plugin for the Unreal Engine 4. 

You can use convenience blueprints to just select and play or use a custom approach via blueprints or C++. See relevant sections for details

You can extend functionality to any blueprint through adding the LeapInterfaceEvent interface then adding the LeapController component to that blueprint. This same architecture is available to C++ if you prefer, which also supports both event-driven and polling style use.

See [unreal thread](https://forums.unrealengine.com/showthread.php?49107-Plugin-Leap-Motion-Event-Driven) for version downloads and development updates.

Since 4.11 this is now the official plugin!

## How to Setup (since 4.11)

1. Create new or open a project. 
2. Select Window->Plugins. Click on the *Input Devices* category and you should see a plugin called Leap Motion. Select Enabled. The Editor will warn you to restart, click restart.
3. The plugin should be enabled and ready to use.

<img src="http://i.imgur.com/h9ib6GP.gif">


## How to use it - Convenience Rigged Characters
Since 0.7.10 the plugin includes convenience content for easy reference of say a rigged setup. Since 0.9 these are automatically included as plugin content.

#### Convenience Characters

To try the rigged or floating hands characters, change your game mode to use *LeapRiggedCharacter* or *LeapFloatingHandsCharacter* as your default pawn.

<img src ="http://i.imgur.com/3jP66IJ.gif">

That's it! hit play to try it out!

<img src ="http://i.imgur.com/HWVaeid.gif">
<br>

If you wish to toggle floating hand types, add *ChangeHands* to your input action mapping, e.g. using the keyboard button *H*

#### Engine Plugin Vive Fix

If you're using the plugin with a Vive in VR mode, adjust your camera offset to make the hands match where you should see them

<img src ="http://i.imgur.com/gvEJycT.png">

#### Add Convenience Leap Hands to Custom Character

1. Add a camera to your pawn/character, parent e.g. *LeapRiggedEchoHandsActor* or *LeapImageHandsActor* to that camera as a child actor.
2. Optionally modify parameters
3. Hit play!

#### Collision Character
Since 2.0 the *LeapRiggedCharacter* and *LeapFloatingHandsCharacter* automatically have collision enabled.

If you want to modify the passthrough character or your own sub-class to have collision simply change one setting in your pawn.

First select your mesh

<img src ="http://i.imgur.com/zBzgWaF.png">

Change the collision preset to PhysicsActor

<img src= "http://i.imgur.com/dAsdcEo.png">

This will use the BasicBody_Physics PhysicsAsset included with the plugin content for collision. You can modify the collision shapes there to modify what part of your mesh collides with the world.

<img src="http://i.imgur.com/g9oChrM.png">

Using collision you can now interact directly with movable actors with physics simulation turned on.

<img src="http://i.imgur.com/IVEQ2fp.gif">

#### Passthrough Character

Pass-through is supported from 0.9, useful if you wish to blend real-world for AR purposes or simply wish to see where you type in vr. 
Simply select LeapPassthroughCharacter as your pawn.

<img src ="http://i.imgur.com/D7ifDlj.gif">

Play to try out AR/VR transition with a simple gesture

<img src ="http://i.imgur.com/ozbhr3E.gif">
<br>
###Convenience Setup and Notes###

Convenience content is built with re-usability in mind. Below is a diagram showing how all the blueprint classes relate.

<img src ="http://i.imgur.com/TQRDmux.png">
Convenience content relation diagram

The LeapBasicRiggedCharacter has only one skeletal mesh and can be used as a basis for basic non-VR characters. If you're rigging a VR character you should split the mesh head from the body so that it can be hidden from the character in order to alleviate clipping issues, this is the functionality that the LeapRiggedCharacter extends from the basic version. Finally if you want to have automatic access to passthrough mode, use the LeapPassthroughCharacter.

What this setup gives you in flexibility is that you have many entry points from which to modify any aspect of the rigging. E.g. If you wish to change post process material being used you would change it in the PPChanger blueprint while still retaining every other functionality. Or if you want to adjust the default mode offset (where your leap is in relation to you when place on the table) you would modify the LeapAnimBodyConnector component which specifies its location. If you want to use your own skeletal mesh, simply replace the mesh, but retain the animation blueprint (use animation retargeting if you're using different skeletons). In fact the rigging setup doesn't even need the leap motion to work, any input which would change the AnimBody skeleton to whatever pose you wish to attain would have the same rigged behavior. The setup is the beginning part of a BodyInput plugin which will support a similar setup for various VR inputs in the future.
<br>
#### Note on Leap Base offset in Default Mode

All rigged characters automatically swap the appropriate leap offset to display the hands in both default mode (leap on table facing up) and HMD mode by detecting when the user enables their HMD (e.g. by going fullscreen in UE4).
<img src="http://i.imgur.com/nEk4d3J.png">

Should you wish to adjust the offset when using the leap on the table facing up, modify the position of the DefaultModeSprite in the LeapAnimBodyConnector. VR mode needs no adjustment as it is always 1:1 due to being attached to your HMD .

## How to use it - Blueprint without Convenience Content, Quick Setup
<ol>
<li>Open desired blueprint where you want to receive the leap events.</li>
<li>Click on Class Settings and Scroll down in the details panel to Interfaces. </li>

<img src="http://i.imgur.com/apyiM2c.png">

<li>Add the interface called LeapEventInterface</li>
<li>Add Leap Controller Component through option A <b>or</b> B</li>

<b>Option A:</b> 
Add directly through Add Components
<img src="http://i.imgur.com/bGoWWbg.png">

<br/>
<b>Option B:</b> Place BeginPlay event on your event graph, drag out from the execute node typing 'Leap', select Add LeapController.</li>
<img src="http://i.imgur.com/y5caGWf.png">
<li>Option B, (Optional, recommended) Drag from the return value, save this as a blueprint variable e.g. Leap.
<li>Both, (Optional) Drag from the return value or your Leap blueprint variable (get), type 'Opt' select the only node which is OptimizeForHMD. Select true for the boolean input (red input), this will optimize the leap for top-down use. Keep auto rotate and auto shift if you want the plugin to automatically shift the leap positions to account for HMD movement.
<li>You're ready to go.

</ol>


## How to use it - Input Mapping for Joystick-like Input
The easiest way to use the Leap Motion is to use the input mapping system for simple, joystick-like maneuvers.

E.g. by adding the one input mapping key and 2 axis to the Rolling Template, you can roll the ball by the pitch and roll of your hand and jump by closing your fist or pinching.

<img src="http://i.imgur.com/MBOQmbO.png">

## How to use it - Blueprint - Event Driven

You can Right click anywhere on Event graph and type 'Leap Event' and you will see all the available events you can receive.

### Example 'Debug Hand' graph
<img src="http://i.imgur.com/VzgR3xC.png">

with

<img src="http://i.imgur.com/aEEuZSI.png">

### Result
<img src="http://i.imgur.com/URnaAHe.png">


### Example Leap Image API 
Supports memory efficient event driven api and leap api style polling

For both cases you need to inform the plugin that you want to receive images.

<img src="http://i.imgur.com/mbQuuGG.png">

Note that you can control whether events are being emitted by toggling Emit Image events. It is recommended to disable both when not in use for performance and bandwidth reasons.

Example of a toggle
<img src="http://i.imgur.com/AE8OBHA.png">

#### Event Driven Example
<img src="http://i.imgur.com/20G6pfa.png">

Which gives the following billboard results (note the finger and palm debug visualization is a result of a separate graph)

<img src="http://i.imgur.com/qRhPHDC.png">

Distortion texture maps and other image parameters can be obtained by branching the LeapImage node (branched in the example graph to obtain the image id).

## Leap API Reference

All the Blueprint nodes and functions keep the same name as specified in the <a href="https://developer.leapmotion.com/documentation/skeletal/cpp/api/Leap.Controller.html">documentation of leapmotion</a>, refer to this documentation for help with the API. Events are custom named, but follow action oriented naming. Type 'Leap Event' in your blueprint event graph to see all possible events after adding the LeapEventInterface to your blueprint interfaces.

### Blueprint API Examples

Not an exhaustive list, but common use list. See the leap motion documentation for full list.

#### Events

<img src="http://i.imgur.com/XI3WmIM.png">

#### Frame Functions

<img src="http://i.imgur.com/Ioe19p0.png">

#### Hand Functions and Properties

<img src="http://i.imgur.com/o340HWj.png">

#### Arm Functions and Properties

<img src="http://i.imgur.com/ymlIeWt.png">

#### Finger Functions and Properties

<img src="http://i.imgur.com/MLblo0F.png">

#### Pointable Functions and Properties

<img src="http://i.imgur.com/NRpgG7z.png">

<br/>
<br/>


#### Leap API Style Polling
The same above result can be achieve by using Leap API style polling like this

<img src="http://i.imgur.com/GxTyG1i.png">

Note that a difference between the two forms occurs when image event toggle is off. The plugin reclaims memory and uses no extra performance in the case of events, whereas polling based example will continue attempting to get a texture even if the allow images is off and will cause the plugin to warn the user to enable images every tick. To get the same functionality as the event driven format a separate boolean would need to be used to branch the execution to stop the log warning spam.

### Gestures
Require enabling before receiving events, below is an example of how to get the Circle gesture to emit and how to filter it.

<img src="http://i.imgur.com/102W2cT.png">

With 0.7.2 the plugin supports the gesture sub-classes directly, here is an example of showing a debug sphere around the circle you are gesturing. Note that you can use the generic gesture detection event and cast the gesture to your sub-gesture or use the sub-gesture specific event. Both are equivalent.leap-ue4
====================

An event-driven [Leap Motion](http://www.leapmotion.com) plugin for the Unreal Engine 4. 

You can use convenience blueprints to just select and play or use a custom approach via blueprints or C++. See relevant sections for details

You can extend functionality to any blueprint through adding the LeapInterfaceEvent interface then adding the LeapController component to that blueprint. This same architecture is available to C++ if you prefer, which also supports both event-driven and polling style use.

See [unreal thread](https://forums.unrealengine.com/showthread.php?49107-Plugin-Leap-Motion-Event-Driven) for version downloads and development updates.

Since 4.11 this is now the official plugin!

## How to Setup (since 4.11

1. Create new or open a project. 
2. Select Window->Plugins. Click on the *Input Devices* category and you should see a plugin called Leap Motion. Select Enabled. The Editor will warn you to restart, click restart.
3. The plugin should be enabled and ready to use.

<img src="http://i.imgur.com/h9ib6GP.gif">


## How to use it - Convenience Rigged Characters
Since 0.7.10 the plugin includes convenience content for easy reference of say a rigged setup. Since 0.9 these are automatically included as plugin content.

#### Convenience Characters

To try the rigged or floating hands characters, change your game mode to use *LeapRiggedCharacter* or *LeapFloatingHandsCharacter* as your default pawn.

<img src ="http://i.imgur.com/3jP66IJ.gif">

That's it! hit play to try it out!

<img src ="http://i.imgur.com/HWVaeid.gif">
<br>

If you wish to toggle floating hand types, add *ChangeHands* to your input action mapping, e.g. using the keyboard button *H*

#### Add Convenience Leap Hands to Custom Character

1. Add a camera to your pawn/character, parent e.g. *LeapRiggedEchoHandsActor* or *LeapImageHandsActor* to that camera as a child actor.
2. Optionally modify parameters
3. Hit play!

#### Collision Character
Since 2.0 the *LeapRiggedCharacter* and *LeapFloatingHandsCharacter* automatically have collision enabled.

If you want to modify the passthrough character or your own sub-class to have collision simply change one setting in your pawn.

First select your mesh

<img src ="http://i.imgur.com/zBzgWaF.png">

Change the collision preset to PhysicsActor

<img src= "http://i.imgur.com/dAsdcEo.png">

This will use the BasicBody_Physics PhysicsAsset included with the plugin content for collision. You can modify the collision shapes there to modify what part of your mesh collides with the world.

<img src="http://i.imgur.com/g9oChrM.png">

Using collision you can now interact directly with movable actors with physics simulation turned on.

<img src="http://i.imgur.com/IVEQ2fp.gif">

#### Passthrough Character

Pass-through is supported from 0.9, useful if you wish to blend real-world for AR purposes or simply wish to see where you type in vr. 
Simply select LeapPassthroughCharacter as your pawn.

<img src ="http://i.imgur.com/D7ifDlj.gif">

Play to try out AR/VR transition with a simple gesture

<img src ="http://i.imgur.com/ozbhr3E.gif">
<br>
### Convenience Setup and Notes

Convenience content is built with re-usability in mind. Below is a diagram showing how all the blueprint classes relate.

<img src ="http://i.imgur.com/TQRDmux.png">
Convenience content relation diagram

The LeapBasicRiggedCharacter has only one skeletal mesh and can be used as a basis for basic non-VR characters. If you're rigging a VR character you should split the mesh head from the body so that it can be hidden from the character in order to alleviate clipping issues, this is the functionality that the LeapRiggedCharacter extends from the basic version. Finally if you want to have automatic access to passthrough mode, use the LeapPassthroughCharacter.

What this setup gives you in flexibility is that you have many entry points from which to modify any aspect of the rigging. E.g. If you wish to change post process material being used you would change it in the PPChanger blueprint while still retaining every other functionality. Or if you want to adjust the default mode offset (where your leap is in relation to you when place on the table) you would modify the LeapAnimBodyConnector component which specifies its location. If you want to use your own skeletal mesh, simply replace the mesh, but retain the animation blueprint (use animation retargeting if you're using different skeletons). In fact the rigging setup doesn't even need the leap motion to work, any input which would change the AnimBody skeleton to whatever pose you wish to attain would have the same rigged behavior. The setup is the beginning part of a BodyInput plugin which will support a similar setup for various VR inputs in the future.
<br>
#### Note on Leap Base offset in Default Mode

All rigged characters automatically swap the appropriate leap offset to display the hands in both default mode (leap on table facing up) and HMD mode by detecting when the user enables their HMD (e.g. by going fullscreen in UE4).
<img src="http://i.imgur.com/nEk4d3J.png">

Should you wish to adjust the offset when using the leap on the table facing up, modify the position of the DefaultModeSprite in the LeapAnimBodyConnector. VR mode needs no adjustment as it is always 1:1 due to being attached to your HMD .

## How to use it - Blueprint without Convenience Content, Quick Setup
<ol>
<li>Open desired blueprint where you want to receive the leap events.</li>
<li>Click on Class Settings and Scroll down in the details panel to Interfaces. </li>

<img src="http://i.imgur.com/apyiM2c.png">

<li>Add the interface called LeapEventInterface</li>
<li>Add Leap Controller Component through option A <b>or</b> B</li>

<b>Option A:</b> 
Add directly through Add Components
<img src="http://i.imgur.com/bGoWWbg.png">

<br/>
<b>Option B:</b> Place BeginPlay event on your event graph, drag out from the execute node typing 'Leap', select Add LeapController.</li>
<img src="http://i.imgur.com/y5caGWf.png">
<li>Option B, (Optional, recommended) Drag from the return value, save this as a blueprint variable e.g. Leap.
<li>Both, (Optional) Drag from the return value or your Leap blueprint variable (get), type 'Opt' select the only node which is OptimizeForHMD. Select true for the boolean input (red input), this will optimize the leap for top-down use. Keep auto rotate and auto shift if you want the plugin to automatically shift the leap positions to account for HMD movement.
<li>You're ready to go.

</ol>


## How to use it - Input Mapping for Joystick-like Input
The easiest way to use the Leap Motion is to use the input mapping system for simple, joystick-like maneuvers.

E.g. by adding the one input mapping key and 2 axis to the Rolling Template, you can roll the ball by the pitch and roll of your hand and jump by closing your fist or pinching.

<img src="http://i.imgur.com/MBOQmbO.png">

## How to use it - Blueprint - Event Driven

You can Right click anywhere on Event graph and type 'Leap Event' and you will see all the available events you can receive.

### Example 'Debug Hand' graph
<img src="http://i.imgur.com/VzgR3xC.png">

with

<img src="http://i.imgur.com/aEEuZSI.png">

### Result
<img src="http://i.imgur.com/URnaAHe.png">


### Example Leap Image API
Supports memory efficient event driven api and leap api style polling

For both cases you need to inform the plugin that you want to receive images.

<img src="http://i.imgur.com/mbQuuGG.png">

Note that you can control whether events are being emitted by toggling Emit Image events. It is recommended to disable both when not in use for performance and bandwidth reasons.

Example of a toggle
<img src="http://i.imgur.com/AE8OBHA.png">

#### Event Driven Example
<img src="http://i.imgur.com/20G6pfa.png">

Which gives the following billboard results (note the finger and palm debug visualization is a result of a separate graph)

<img src="http://i.imgur.com/qRhPHDC.png">

Distortion texture maps and other image parameters can be obtained by branching the LeapImage node (branched in the example graph to obtain the image id).

## Leap API Reference

All the Blueprint nodes and functions keep the same name as specified in the <a href="https://developer.leapmotion.com/documentation/skeletal/cpp/api/Leap.Controller.html">documentation of leapmotion</a>, refer to this documentation for help with the API. Events are custom named, but follow action oriented naming. Type 'Leap Event' in your blueprint event graph to see all possible events after adding the LeapEventInterface to your blueprint interfaces.

### Blueprint API Examples

Not an exhaustive list, but common use list. See the leap motion documentation for full list.

#### Events

<img src="http://i.imgur.com/XI3WmIM.png">

#### Frame Functions

<img src="http://i.imgur.com/Ioe19p0.png">

#### Hand Functions and Properties

<img src="http://i.imgur.com/o340HWj.png">

#### Arm Functions and Properties

<img src="http://i.imgur.com/ymlIeWt.png">

#### Finger Functions and Properties

<img src="http://i.imgur.com/MLblo0F.png">

#### Pointable Functions and Properties

<img src="http://i.imgur.com/NRpgG7z.png">

<br/>
<br/>


#### Leap API Style Polling
The same above result can be achieve by using Leap API style polling like this

<img src="http://i.imgur.com/GxTyG1i.png">

Note that a difference between the two forms occurs when image event toggle is off. The plugin reclaims memory and uses no extra performance in the case of events, whereas polling based example will continue attempting to get a texture even if the allow images is off and will cause the plugin to warn the user to enable images every tick. To get the same functionality as the event driven format a separate boolean would need to be used to branch the execution to stop the log warning spam.

### Gestures
Require enabling before receiving events, below is an example of how to get the Circle gesture to emit and how to filter it.

<img src="http://i.imgur.com/102W2cT.png">

With 0.7.2 the plugin supports the gesture sub-classes directly, here is an example of showing a debug sphere around the circle you are gesturing. Note that you can use the generic gesture detection event and cast the gesture to your sub-gesture or use the sub-gesture specific event. Both are equivalent.

<img src="http://i.imgur.com/hQBIYTH.png">

## How to use it - Blueprint - Polling

When you create a class blueprint you can add a component called LeapController, this contains all the functionality that the LeapController has specified in their API. From the controller you can get the frame and do what you want with features currently available.<br/>
<br/>
Available blueprint classes branching from LeapController:
<ul>
<li>Arm</li>
<li>Bone</li>
<li>CircleGesture</li>
<li>Finger</li>
<li>FingerList</li>
<li>Frame</li>
<li>Gesture</li>
<li>GestureList</li>
<li>Hand</li>
<li>HandList</li>
<li>InteractionBox</li>
<li>KeyTapGesture</li>
<li>LeapImage</li> 
<li>LeapImageList</li> 
<li>Pointable</li>
<li>PointableList</li>
<li>ScreenTapGesture</li>
<li>SwipeGesture</li>
<li>Tool</li>
<li>ToolList</li>
</ul>

### Shipping Troubleshooting
- If you get errors complaining 'Leap Motion' plugin cannot be found, you do not have code in your project.
- If you get errors complaining the 'Leap.dll' is missing, you did not drag binaries to the correct location.

## Todo

- C++ how to

## Contact
Post issues to this github or [the unreal thread](https://forums.unrealengine.com/showthread.php?49107-Plugin-Leap-Motion-Event-Driven).

## Credits
Event-driven fork by Getnamo.

[Original Plugin](https://github.com/wieser-m/UE4-LeapMotionPlugin) by Marc Wieser.

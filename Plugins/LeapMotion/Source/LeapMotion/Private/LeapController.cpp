#include "LeapMotionPrivatePCH.h"
#include "LeapEventInterface.h"
#include "SlateBasics.h"
#include <vector>

#include <sstream> //for std::stringstream 
#include <string>  //for std::string

//#include "ILeapMotion.h"

#define LEAP_IM_SCALE 0.01111111111 //this is 1/90. We consider 90 degrees to be 1.0 for Input mapping

//Global controller count, temporary debug variable since the plugin does not support more than one component. We track it and warn the users.
int controllerCount = 0;

//Input mapping - FKey declarations
//Define each FKey const in a .cpp so we can compile
const FKey EKeysLeap::LeapLeftPinch("LeapLeftPinch");
const FKey EKeysLeap::LeapLeftGrab("LeapLeftGrab");
const FKey EKeysLeap::LeapLeftPalmPitch("LeapLeftPalmPitch");
const FKey EKeysLeap::LeapLeftPalmYaw("LeapLeftPalmYaw");
const FKey EKeysLeap::LeapLeftPalmRoll("LeapLeftPalmRoll");

const FKey EKeysLeap::LeapRightPinch("LeapRightPinch");
const FKey EKeysLeap::LeapRightGrab("LeapRightGrab");
const FKey EKeysLeap::LeapRightPalmPitch("LeapRightPalmPitch");
const FKey EKeysLeap::LeapRightPalmYaw("LeapRightPalmYaw");
const FKey EKeysLeap::LeapRightPalmRoll("LeapRightPalmRoll");

//UE v4.6 IM event wrappers
bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int32 user, bool repeat)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}


//Used for Reliable State Tracking
struct LeapHandStateData{
	bool grabbed;
	bool pinched;
	int32 fingerCount;
	int32 id;
};

class LeapStateData{
public:
	bool hasStateForId(int32 hId, LeapHandStateData& state);
	LeapHandStateData stateForId(int32 hId);
	void setStateForId(LeapHandStateData handState, int32 hId);

	std::vector<LeapHandStateData> handStates;
	int32 handCount;
};

//LeapStateData
bool LeapStateData::hasStateForId(int32 hId, LeapHandStateData& state)
{
	for (size_t i = 0; i < handStates.size(); ++i) {
		// If two Myo pointers compare equal, they refer to the same Myo device.
		if (handStates[i].id == hId) {
			state = handStates[i];
			return true;
		}
	}
	return false;
}

LeapHandStateData LeapStateData::stateForId(int32 hId)
{
	LeapHandStateData handState;
	if (!hasStateForId(hId, handState)){
		handState.id = hId;
		handStates.push_back(handState);
	}

	return handState;
}

void LeapStateData::setStateForId(LeapHandStateData handState, int32 hId)
{
	for (size_t i = 0; i < handStates.size(); ++i) {
		// If two Myo pointers compare equal, they refer to the same Myo device.
		if (handStates[i].id == hId) {
			handStates[i] = handState;
		}
	}
}

class LeapControllerPrivate
{
public:
	~LeapControllerPrivate()
	{
		if(!cleanupCalled)
			CleanupEventData();
	}

	//Ensures our rooted objects are unrooted so they can be GC'd
	void CleanupEventData()
	{
		if (eventHand)
			eventHand->CleanupRootReferences();
		if (eventFinger)
			eventFinger->CleanupRootReferences();
		if (eventGesture)
			eventGesture->CleanupRootReferences();
		if (eventCircleGesture)
			eventCircleGesture->CleanupRootReferences();
		if (eventKeyTapGesture)
			eventKeyTapGesture->CleanupRootReferences();
		if (eventScreenTapGesture)
			eventScreenTapGesture->CleanupRootReferences();
		if (eventSwipeGesture)
			eventSwipeGesture->CleanupRootReferences();
		if (eventImage1)
			eventImage1->CleanupRootReferences();
		if (eventImage2)
			eventImage2->CleanupRootReferences();
		if (frame)
			frame->CleanupRootReferences();
		cleanupCalled = true;
		UE_LOG(LeapPluginLog, Log, TEXT("LeapController::CleanupEventData Delete Called"));
	}
	bool cleanupCalled = false;

	//Properties and Pointers
	LeapStateData pastState;
	Leap::Controller leap;
	ULeapFrame* frame = NULL;

	//Event UObjects, we have to manage memory for these to stop leaks
	ULeapHand* eventHand = NULL;
	ULeapFinger* eventFinger = NULL;
	ULeapGesture* eventGesture = NULL;
	ULeapCircleGesture* eventCircleGesture = NULL;
	ULeapKeyTapGesture* eventKeyTapGesture = NULL;
	ULeapScreenTapGesture* eventScreenTapGesture = NULL;
	ULeapSwipeGesture* eventSwipeGesture = NULL;
	ULeapImage* eventImage1 = NULL;
	ULeapImage* eventImage2 = NULL;
	
	UObject* interfaceDelegate = NULL;	//NB they have to be set to null manually or MSFT will set them to CDCDCDCD...
	bool optimizeForHMD = false;
	bool allowImages = false;
	bool imageEventsEnabled;

	//Functions
	void SetPolicyStatus(Leap::Controller::PolicyFlag flag, bool status);
	void SetPolicyFlagsFromBools();
};

//LeapControllerPrivate
void LeapControllerPrivate::SetPolicyStatus(Leap::Controller::PolicyFlag flag, bool status)
{
	if (status)
		leap.setPolicy(flag);
	else
		leap.clearPolicy(flag);
}

void LeapControllerPrivate::SetPolicyFlagsFromBools()
{
	SetPolicyStatus(Leap::Controller::PolicyFlag::POLICY_OPTIMIZE_HMD, optimizeForHMD);
	SetPolicyStatus(Leap::Controller::PolicyFlag::POLICY_IMAGES, allowImages);
}

//ULeapController
ULeapController::ULeapController(const FObjectInitializer &init) : UActorComponent(init), _private(new LeapControllerPrivate())
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;	//the component automatically ticks so we don't have to
}

ULeapController::~ULeapController()
{
	delete _private;
}

bool ULeapController::IsConnected() const
{
	return _private->leap.isConnected();
}

void ULeapController::OnRegister()
{
	Super::OnRegister();

	//Attach the delegate pointer automatically
	SetInterfaceDelegate(GetOwner());		

	//Track and warn users about multiple components.
	controllerCount++;
	if (controllerCount>1)
		UE_LOG(LeapPluginLog, Warning, TEXT("Warning! More than one (%d) Leap Controller Component detected! Duplication of work and output may result (inefficiency warning)."), controllerCount);

	//UE_LOG(LeapPluginLog, Log, TEXT("Registered Leap Component(%d)."), controllerCount);
}

void ULeapController::OnUnregister()
{
	//Allow GC of private UObject data
	_private->CleanupEventData();	//UObjects attached to root need to be unrooted before we unregister so we do not crash between pie sessions (or without attaching to this object, leak them)
	controllerCount--;

	Super::OnUnregister();
	//UE_LOG(LeapPluginLog, Log, TEXT("Unregistered Leap Component(%d)."), controllerCount);
}

void ULeapController::TickComponent(float DeltaTime, enum ELevelTick TickType,
									FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Forward the component tick
	InterfaceEventTick(DeltaTime);
}

ULeapFrame* ULeapController::Frame(int32 history)
{
	if (_private->frame == NULL)
	{
		_private->frame = NewObject<ULeapFrame>(this, ULeapFrame::StaticClass());
		_private->frame->SetFlags(RF_RootSet);
	}
	_private->frame->setFrame(_private->leap, history);
	return (_private->frame);
}

bool ULeapController::HasFocus() const
{
	return (_private->leap.hasFocus());
}

bool ULeapController::IsServiceConnected() const
{
	return (_private->leap.isServiceConnected());
}


void ULeapController::OptimizeForHMD(bool useTopdown, bool autoRotate, bool autoShift)
{
	//Set Policy Optimization
	_private->optimizeForHMD = useTopdown;
	_private->SetPolicyFlagsFromBools();

	//Pass adjustment booleans
	if (useTopdown)
		LeapSetShouldAdjustForFacing(true);
	else
		LeapSetShouldAdjustForFacing(false);

	LeapSetShouldAdjustForHMD(autoRotate, autoShift);
	//LeapSetShouldAdjustForMountOffset(true);	//this function defaults to true at the moment
}

void ULeapController::EnableImageSupport(bool allowImages, bool emitImageEvents)
{
	_private->allowImages = allowImages;
	_private->SetPolicyFlagsFromBools();
	_private->imageEventsEnabled = emitImageEvents;
}

void ULeapController::EnableBackgroundTracking(bool trackInBackground)
{
	_private->SetPolicyStatus(Leap::Controller::PolicyFlag::POLICY_BACKGROUND_FRAMES, trackInBackground);
}

void ULeapController::EnableGesture(LeapGestureType type, bool enable)
{
	Leap::Gesture::Type rawType;

	switch (type)
	{
	case GESTURE_TYPE_CIRCLE:
		rawType = Leap::Gesture::TYPE_CIRCLE;
		break;
	case GESTURE_TYPE_KEY_TAP:
		rawType = Leap::Gesture::TYPE_KEY_TAP;
		break;
	case GESTURE_TYPE_SCREEN_TAP:
		rawType = Leap::Gesture::TYPE_SCREEN_TAP;
		break;
	case GESTURE_TYPE_SWIPE:
		rawType = Leap::Gesture::TYPE_SWIPE;
		break;
	default:
		rawType = Leap::Gesture::TYPE_INVALID;
		break;
	}

	_private->leap.enableGesture(rawType, enable);
}

//Leap Event Interface - Event Driven System
void ULeapController::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LeapPluginLog, Log, TEXT("InterfaceObject: %s"), *newDelegate->GetName());

	//Use this format to support both blueprint and C++ form
	if (newDelegate->GetClass()->ImplementsInterface(ULeapEventInterface::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("LeapController Warning: Delegate is set"));
		_private->interfaceDelegate = newDelegate; // Blueprint Interface method
		EventInterfaceCpp = Cast<ILeapEventInterface>(_private->interfaceDelegate); // Cpp Interface method (to allow override from c++)
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("LeapController Warning: Delegate is NOT set, did you implement LeapEventInterface?"));
	}
}


//Utility
bool handClosed(float strength)
{
	return (strength == 1.f);
}

bool handPinched(float strength)
{
	return (strength > 0.8);
}

bool handForId(int32 checkId, Leap::HandList hands, Leap::Hand& returnHand)
{
	for (int i = 0; i < hands.count(); i++)
	{
		Leap::Hand hand = hands[i];
		if (checkId == hand.id()){
			returnHand = hand;
			return true;
		}
	}
	return false;
}



//Main Event driven tick
void ULeapController::InterfaceEventTick(float DeltaTime)
{
	//This is our tick event that is forwarded from the delegate, check validity
	if (!_private->interfaceDelegate) return;

	//Pointers
	Leap::Frame frame = _private->leap.frame();
	Leap::Frame pastFrame = _private->leap.frame(1);

	//-Hands-

	//Hand Count
	int handCount = frame.hands().count();

	if (_private->pastState.handCount != handCount)
	{
		//UE_LOG(LeapPluginLog, Log, TEXT("Execute_HandCountChanged"));
		if(EventInterfaceCpp)
		{
			//UE_LOG(LeapPluginLog, Log, TEXT("Execute_HandCountChanged: %s"), *EventInterfaceCpp->ToString());
			EventInterfaceCpp->OnHandCountChanged(handCount);
		}
		else {
			//UE_LOG(LeapPluginLog, Log, TEXT("Execute_HandCountChanged: Interface c++ not set, calling blueprint."));
			ILeapEventInterface::Execute_HandCountChanged(_private->interfaceDelegate, handCount);
		}
		
		//Zero our input mapping orientations (akin to letting go of a joystick)
		if (handCount == 0)
		{
			EmitAnalogInputEventForKey(EKeysLeap::LeapLeftPalmPitch, 0, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapLeftPalmYaw, 0, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapLeftPalmRoll, 0, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapRightPalmPitch, 0, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapRightPalmYaw, 0, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapRightPalmRoll, 0, 0, 0);
		}
	}
	
	//Cycle through each hand
	for (int i = 0; i < handCount; i++)
	{
		Leap::Hand hand = frame.hands()[i];
		LeapHandStateData pastHandState = _private->pastState.stateForId(hand.id());		//we use a custom class to hold reliable state tracking based on id's

		//Make a ULeapHand
		if (_private->eventHand == NULL)
		{
			_private->eventHand = NewObject<ULeapHand>(this);
			_private->eventHand->SetFlags(RF_RootSet);
		}
		_private->eventHand->setHand(hand);

		//Emit hand
		if(EventInterfaceCpp)
		{
			//UE_LOG(LeapPluginLog, Log, TEXT("Calling OnLeapHandMovedCpp: %s"), *EventInterfaceCpp->ToString());
			EventInterfaceCpp->OnLeapHandMoved(_private->eventHand);
		}
		ILeapEventInterface::Execute_LeapHandMoved(_private->interfaceDelegate, _private->eventHand);

		//Left/Right hand forwarding
		if (hand.isRight())
		{
			ILeapEventInterface::Execute_LeapRightHandMoved(_private->interfaceDelegate, _private->eventHand);
			//Input Mapping
			FRotator palmOrientation = _private->eventHand->PalmOrientation;
			EmitAnalogInputEventForKey(EKeysLeap::LeapRightPalmPitch, palmOrientation.Pitch * LEAP_IM_SCALE, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapRightPalmYaw, palmOrientation.Yaw * LEAP_IM_SCALE, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapRightPalmRoll, palmOrientation.Roll * LEAP_IM_SCALE, 0, 0);
		} else if (hand.isLeft())
		{
			ILeapEventInterface::Execute_LeapLeftHandMoved(_private->interfaceDelegate, _private->eventHand);
			//Input Mapping
			FRotator palmOrientation = _private->eventHand->PalmOrientation;
			EmitAnalogInputEventForKey(EKeysLeap::LeapLeftPalmPitch, palmOrientation.Pitch * LEAP_IM_SCALE, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapLeftPalmYaw, palmOrientation.Yaw * LEAP_IM_SCALE, 0, 0);
			EmitAnalogInputEventForKey(EKeysLeap::LeapLeftPalmRoll, palmOrientation.Roll * LEAP_IM_SCALE, 0, 0);
		}
		
		//Grabbing
		float grabStrength = hand.grabStrength();
		bool grabbed = handClosed(grabStrength);

		if (grabbed)
			ILeapEventInterface::Execute_LeapHandGrabbing(_private->interfaceDelegate, grabStrength, _private->eventHand);

		if (grabbed && !pastHandState.grabbed)
		{
			ILeapEventInterface::Execute_LeapHandGrabbed(_private->interfaceDelegate, grabStrength, _private->eventHand);
			
			//input mapping
			if (_private->eventHand->HandType == LeapHandType::HAND_LEFT)
				EmitKeyDownEventForKey(EKeysLeap::LeapLeftGrab, 0, 0);
			else
				EmitKeyDownEventForKey(EKeysLeap::LeapRightGrab, 0, 0);
		}else if (!grabbed && pastHandState.grabbed)
		{
			ILeapEventInterface::Execute_LeapHandReleased(_private->interfaceDelegate, grabStrength, _private->eventHand);

			//input mapping
			if (_private->eventHand->HandType == LeapHandType::HAND_LEFT)
				EmitKeyUpEventForKey(EKeysLeap::LeapLeftGrab, 0, 0);
			else
				EmitKeyUpEventForKey(EKeysLeap::LeapRightGrab, 0, 0);
		}

		//Pinching
		float pinchStrength = hand.pinchStrength();
		bool pinched = handPinched(pinchStrength);

		//While grabbing disable pinching detection, this helps to reduce spam as pose confidence plummets
		if (grabbed) pinched = pastHandState.pinched;
		else
		{
			if (pinched)
				ILeapEventInterface::Execute_LeapHandPinching(_private->interfaceDelegate, pinchStrength, _private->eventHand);

			if (pinched && !pastHandState.pinched)
			{
				ILeapEventInterface::Execute_LeapHandPinched(_private->interfaceDelegate, pinchStrength, _private->eventHand);
				//input mapping
				if (_private->eventHand->HandType == LeapHandType::HAND_LEFT)
					EmitKeyDownEventForKey(EKeysLeap::LeapLeftPinch, 0, 0);
				else
					EmitKeyDownEventForKey(EKeysLeap::LeapRightPinch, 0, 0);
			}
			else if (!pinched && pastHandState.pinched)
			{
				ILeapEventInterface::Execute_LeapHandUnpinched(_private->interfaceDelegate, pinchStrength, _private->eventHand);
				//input mapping
				if (_private->eventHand->HandType == LeapHandType::HAND_LEFT)
					EmitKeyUpEventForKey(EKeysLeap::LeapLeftPinch, 0, 0);
				else
					EmitKeyUpEventForKey(EKeysLeap::LeapRightPinch, 0, 0);
			}
		}
	
		//-Fingers-
		Leap::FingerList fingers = hand.fingers();

		//Count
		int fingerCount = fingers.count();
		if ((pastHandState.fingerCount != fingerCount))
			ILeapEventInterface::Execute_FingerCountChanged(_private->interfaceDelegate, fingerCount);

		if (_private->eventFinger == NULL)
		{
			_private->eventFinger = NewObject<ULeapFinger>(this);
			_private->eventFinger->SetFlags(RF_RootSet);
		}

		Leap::Finger finger;

		//Cycle through each finger
		for (int j = 0; j < fingerCount; j++)
		{
			finger = fingers[j];
			_private->eventFinger->setFinger(finger);

			//Finger Moved
			if (finger.isValid())
				ILeapEventInterface::Execute_LeapFingerMoved(_private->interfaceDelegate, _private->eventFinger);
		}

		//Do these last so we can easily override debug shapes

		//Leftmost
		finger = fingers.leftmost();
		_private->eventFinger->setFinger(finger);
		ILeapEventInterface::Execute_LeapLeftMostFingerMoved(_private->interfaceDelegate, _private->eventFinger);

		//Rightmost
		finger = fingers.rightmost();
		_private->eventFinger->setFinger(finger);
		ILeapEventInterface::Execute_LeapRightMostFingerMoved(_private->interfaceDelegate, _private->eventFinger);

		//Frontmost
		finger = fingers.frontmost();
		_private->eventFinger->setFinger(finger);
		ILeapEventInterface::Execute_LeapFrontMostFingerMoved(_private->interfaceDelegate, _private->eventFinger);

		//touch only for front-most finger, most common use case
		float touchDistance = finger.touchDistance();
		if (touchDistance <= 0.f)
			ILeapEventInterface::Execute_LeapFrontFingerTouch(_private->interfaceDelegate, _private->eventFinger);

		//Set the state data for next cycle
		pastHandState.grabbed = grabbed;
		pastHandState.pinched = pinched;
		pastHandState.fingerCount = fingerCount;

		_private->pastState.setStateForId(pastHandState, hand.id());
	}
	
	_private->pastState.handCount = handCount;
	
	
	//Gestures
	for (int i = 0; i < frame.gestures().count(); i++)
	{
		Leap::Gesture gesture = frame.gestures()[i];
		Leap::Gesture::Type type = gesture.type();

		switch (type)
		{
		case Leap::Gesture::TYPE_CIRCLE:
			if (_private->eventCircleGesture == NULL){
				_private->eventCircleGesture = NewObject<ULeapCircleGesture>(this);
				_private->eventCircleGesture->SetFlags(RF_RootSet);
			}
			_private->eventCircleGesture->setGesture(Leap::CircleGesture(gesture));
			ILeapEventInterface::Execute_CircleGestureDetected(_private->interfaceDelegate, _private->eventCircleGesture);
			_private->eventGesture = _private->eventCircleGesture;
			break;
		case Leap::Gesture::TYPE_KEY_TAP:
			if (_private->eventKeyTapGesture == NULL)
			{
				_private->eventKeyTapGesture = NewObject<ULeapKeyTapGesture>(this);
				_private->eventKeyTapGesture->SetFlags(RF_RootSet);
			}
			_private->eventKeyTapGesture->setGesture(Leap::KeyTapGesture(gesture));
			ILeapEventInterface::Execute_KeyTapGestureDetected(_private->interfaceDelegate, _private->eventKeyTapGesture);
			_private->eventGesture = _private->eventKeyTapGesture;
			break;
		case Leap::Gesture::TYPE_SCREEN_TAP:
			if (_private->eventScreenTapGesture == NULL)
			{
				_private->eventScreenTapGesture = NewObject<ULeapScreenTapGesture>(this);
				_private->eventScreenTapGesture->SetFlags(RF_RootSet);
			}
			_private->eventScreenTapGesture->setGesture(Leap::ScreenTapGesture(gesture));
			ILeapEventInterface::Execute_ScreenTapGestureDetected(_private->interfaceDelegate, _private->eventScreenTapGesture);
			_private->eventGesture = _private->eventScreenTapGesture;
			break;
		case Leap::Gesture::TYPE_SWIPE:
			if (_private->eventSwipeGesture == NULL)
			{
				_private->eventSwipeGesture = NewObject<ULeapSwipeGesture>(this);
				_private->eventSwipeGesture->SetFlags(RF_RootSet);
			}
			_private->eventSwipeGesture->setGesture(Leap::SwipeGesture(gesture));
			ILeapEventInterface::Execute_SwipeGestureDetected(_private->interfaceDelegate, _private->eventSwipeGesture);
			_private->eventGesture = _private->eventSwipeGesture;
			break;
		default:
			break;
		}

		//emit gesture
		if (type != Leap::Gesture::TYPE_INVALID)
		{
			ILeapEventInterface::Execute_GestureDetected(_private->interfaceDelegate, _private->eventGesture);
		}
	}

	//Image
	if (_private->allowImages && _private->imageEventsEnabled)
	{
		int imageCount = frame.images().count();
		for (int i = 0; i < imageCount; i++)
		{
			Leap::Image image = frame.images()[i];

			//Loop modification - Only emit 0 and 1, use two different pointers so we can get different images
			if (i == 0)
			{
				if (_private->eventImage1 == NULL)
				{
					_private->eventImage1 = NewObject<ULeapImage>(this);
					_private->eventImage1->SetFlags(RF_RootSet);
				}
				_private->eventImage1->setLeapImage(image);

				ILeapEventInterface::Execute_RawImageReceived(_private->interfaceDelegate, _private->eventImage1->Texture(), _private->eventImage1);
			}
			else if (i == 1)
			{
				if (_private->eventImage2 == NULL)
                {
					_private->eventImage2 = NewObject<ULeapImage>(this);
					_private->eventImage2->SetFlags(RF_RootSet);
				}
				_private->eventImage2->setLeapImage(image);

				ILeapEventInterface::Execute_RawImageReceived(_private->interfaceDelegate, _private->eventImage2->Texture(), _private->eventImage2);
			}
		
	}}
}
//header
#include "animationOgre.h"

//shape
#include "../../shape/shape.h"

//applicationBreslin
#include "../../application/applicationBreslin.h"

//game
#include "../../game/game.h"

//states
#include "../states/animationStates.h"

AnimationOgre::AnimationOgre(Shape* shape) : AnimationBreslin(shape)
{
	mShape = shape;

	mAnimationFadeSpeed = 7.5;

	setupAnimations();
}

AnimationOgre::~AnimationOgre()
{
}

void AnimationOgre::setupAnimations()
{
	/*********  setup animations ***************/
    // this is very important due to the nature of the exported animations
    mShape->mEntity->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);
    String animNames[] =
    {"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
    "SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	// populate our animation list
    for (int i = 0; i < mNumberOfAnimations; i++)
    {
		mAnims[i] = mShape->mEntity->getAnimationState(animNames[i]);
        mAnims[i]->setLoop(true);
        mFadingIn[i] = false;
        mFadingOut[i] = false;
    }
    // start off in the idle state (top and bottom together)
    setBaseAnimation(ANIM_IDLE_BASE,false);
    setTopAnimation(ANIM_IDLE_TOP,false);

    // relax the hands since we're not holding anything
    mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void AnimationOgre::runAnimations()
{
	//for now let's slow animations down....
	float tempSpeed = mShape->mSpeed;
	tempSpeed = tempSpeed / 3.0f;

	mAnims[mBaseAnimID]->addTime(mShape->mApplication->getRenderTime() * tempSpeed * 1000/mShape->mSpeedMax );
	mAnims[mTopAnimID]->addTime(mShape->mApplication->getRenderTime() * tempSpeed * 1000/mShape->mSpeedMax );
	fadeAnimations(mShape->mApplication->getRenderTime());
}

void AnimationOgre::enterAnimationState(State<Animation>* s)
{
	
	if (mStateMachine->currentState() == IDLE_INTERPOLATETICK_ANIMATION::Instance())
	{
		// start off in the idle state (top and bottom together)
		setBaseAnimation(ANIM_IDLE_BASE,false);
		setTopAnimation(ANIM_IDLE_TOP,false);

		// relax the hands since we're not holding anything
		mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
	}
	else if (mStateMachine->currentState() == RUN_INTERPOLATETICK_ANIMATION::Instance())
	{
		setBaseAnimation(ANIM_RUN_BASE, true);
	    setTopAnimation(ANIM_RUN_TOP, true);

		// relax the hands since we're not holding anything
		if (!mAnims[ANIM_HANDS_RELAXED]->getEnabled())
		{
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
		}
	}
}

void AnimationOgre::fadeAnimations(Real deltaTime)
{
	for (int i = 0; i < mNumberOfAnimations; i++)
    {
		if (mFadingIn[i])
        {
			// slowly fade this animation in until it has full weight
            Real newWeight = mAnims[i]->getWeight() + deltaTime * mAnimationFadeSpeed;
            mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
            if (newWeight >= 1)
            {
				mFadingIn[i] = false;
            }
        }
        else if (mFadingOut[i])
        {
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[i]->getWeight() - deltaTime * mAnimationFadeSpeed;
            mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
            if (newWeight <= 0)
            {
				mAnims[i]->setEnabled(false);
                mFadingOut[i] = false;
            }
        }
	}
}

void AnimationOgre::setBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < mNumberOfAnimations)
    {
		// if we have an old animation, fade it out
        mFadingIn[mBaseAnimID] = false;
        mFadingOut[mBaseAnimID] = true;
    }
     
    mBaseAnimID = id;

    if (id != ANIM_NONE)
    {
		// if we have a new animation, enable it and fade it in
        mAnims[id]->setEnabled(true);
        mAnims[id]->setWeight(0);
        mFadingOut[id] = false;
        mFadingIn[id] = true;

		if (reset)
        {
			mAnims[id]->setTimePosition(0);
        }
	}
}

void AnimationOgre::setTopAnimation(AnimID id, bool reset)
{

	if (mTopAnimID >= 0 && mTopAnimID < mNumberOfAnimations)
    {
		// if we have an old animation, fade it out
        mFadingIn[mTopAnimID] = false;
        mFadingOut[mTopAnimID] = true;
    }

    mTopAnimID = id;

    if (id != ANIM_NONE)
    {
		// if we have a new animation, enable it and fade it in
        mAnims[id]->setEnabled(true);
        mAnims[id]->setWeight(0);
        mFadingOut[id] = false;
        mFadingIn[id] = true;

        if (reset)
        {
			mAnims[id]->setTimePosition(0);
        }
	}
}
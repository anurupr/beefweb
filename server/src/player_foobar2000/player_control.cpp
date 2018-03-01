#include "player.hpp"

namespace msrv {
namespace player_foobar2000 {

PlayerStatePtr PlayerImpl::queryPlayerState(TrackQuery* activeItemQuery)
{
    return std::make_unique<PlayerState>();
}

void PlayerImpl::playCurrent()
{
    playbackControl_->play_or_unpause();
}

void PlayerImpl::playItem(const PlaylistRef& playlist, int32_t itemIndex)
{
}

void PlayerImpl::playRandom()
{
}

void PlayerImpl::playNext()
{
}

void PlayerImpl::playPrevious()
{
}

void PlayerImpl::stop()
{
}

void PlayerImpl::pause()
{
}

void PlayerImpl::togglePause()
{
}

void PlayerImpl::setMuted(Switch val)
{
}

void PlayerImpl::seekAbsolute(double offsetSeconds)
{
}

void PlayerImpl::seekRelative(double offsetSeconds)
{
}

void PlayerImpl::setVolumeDb(double val)
{
}

void PlayerImpl::setVolumeAmp(double val)
{
}

TrackQueryPtr PlayerImpl::createTrackQuery(const std::vector<std::string>& columns)
{
    return TrackQueryPtr();
}

}}
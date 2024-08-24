#include"strings.h"

std::string RTCPeerConnectionStateToString(rtc::PeerConnection::State state){
	switch(state){
		case rtc::PeerConnection::State::New:return "new";
		case rtc::PeerConnection::State::Connecting:return "connecting";
		case rtc::PeerConnection::State::Connected:return "connected";
		case rtc::PeerConnection::State::Disconnected:return "disconnected";
		case rtc::PeerConnection::State::Failed:return "failed";
		case rtc::PeerConnection::State::Closed:return "closed";
		default:return "unknown";
	}
}

std::string RTCPeerConnectionGatheringStateToString(rtc::PeerConnection::GatheringState state){
	switch(state){
		case rtc::PeerConnection::GatheringState::New:return "new";
		case rtc::PeerConnection::GatheringState::InProgress:return "inprogress";
		case rtc::PeerConnection::GatheringState::Complete:return "complete";
		default:return "unknown";
	}
}

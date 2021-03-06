/* Copyright (c) 2014 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "RpcTracker.h"

namespace RAMCloud {

/**
 * Clean up allocated resources.
 */
RpcTracker::~RpcTracker() {
}

/**
 * Mark the receipt of the result for an RPC.
 *
 * \param rpcId
 *      The id of an Rpc.
 */
void
RpcTracker::rpcFinished(uint64_t rpcId) {
    assert(!rpcs[rpcId % windowSize]);
    rpcs[rpcId % windowSize] = true;
    if (firstMissing == rpcId) {
        firstMissing++;
        while (rpcs[firstMissing % windowSize] && firstMissing < nextRpcId)
            firstMissing++;
    }
}

/**
 * Gets a unique RPC id for new linearizable RPC.
 *
 * \return
 *      The id for new RPC.
 *      Or 0 if the rpc waiting for response (first Missing) is too far behind.
 */
uint64_t
RpcTracker::newRpcId() {
    if (firstMissing + windowSize == nextRpcId) {
        return 0;
    }
    rpcs[nextRpcId % windowSize] = false;
    return nextRpcId++;
}

/**
 * Gets the current acknowledgment id, which indicates RPCs with id smaller
 * than this number are all received results.
 *
 * \return
 *      The ackId value to be sent with new RPC.
 */
uint64_t
RpcTracker::ackId() {
    return firstMissing - 1;
}

} // namespace RAMCloud

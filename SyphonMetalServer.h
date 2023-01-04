#import <Syphon/SyphonServerBase.h>
#import <Metal/MTLPixelFormat.h>
#import <Metal/MTLTexture.h>
#import <Metal/MTLCommandBuffer.h>

NS_ASSUME_NONNULL_BEGIN

/*! @name Server Options Dictionary Key Constants */
/*! @{ */
/*!
 @relates SyphonServerBase
 If this key is matched with a NSNumber with a BOOL value YES, then the server will be invisible to other Syphon users. You are then responsible for passing the NSDictionary returned by serverDescription to processes which require it to create a SyphonClient. Default is NO.
 */
extern NSString * const SyphonServerOptionIsPrivate;

/*! @} */

/*!
 @nosubgrouping
 SyphonMetalServer handles the publishing of frames from one video source to any number of clients. Frames are published by passing in an existing Metal texture.
 
 A SyphonMetalServer object represents one video output for your application. If your application produces several video outputs, then they should each have their own SyphonMetalServer. If your application might have multiple servers running, you should name each server to aid identification by users.
 
 It is safe to access instances of this class across threads.
 */

#define SYPHON_METAL_SERVER_UNIQUE_CLASS_NAME SYPHON_UNIQUE_CLASS_NAME(SyphonMetalServer)
@interface SYPHON_METAL_SERVER_UNIQUE_CLASS_NAME : SyphonServerBase

/** @name Instantiation */
/** @{ */
/*!
 Creates a new server with the specified human-readable name (which need not be unique) for a MTLDevice and given options. The server will be started immediately. Init may fail and return nil if the server could not be started.

 @param name Non-unique human readable server name. This is not required and may be nil, but is usually used by clients in their UI to aid identification.
 @param device The MTLDevice that textures will be valid and available on for publishing.
 @param options A dictionary containing key-value pairs to specify options for the server. The only currently supported option is SyphonServerOptionIsPrivate. See its description for details.
 @returns A newly intialized SyphonMetalServer. Nil on failure.
*/
- (id)initWithName:(nullable NSString*)name device:(id<MTLDevice>)device options:(nullable NSDictionary *)options;

/** @} */

/** @name Properties */
/** @{ */
/*!
 The MTLDevice the server uses for drawing.
*/
@property (readonly) id<MTLDevice> device;

/*!
A string representing the name of the SyphonMetalServer.
*/
@property (nullable, retain) NSString* name;

/*!
A dictionary describing the server. Normally you won't need to access this, however if you created the server as private (using SyphonServerOptionIsPrivate) then you must pass this dictionary to any process in which you wish to create a SyphonClient. You should not rely on the presence of any particular keys in this dictionary. The content will always conform to the \<NSCoding\> protocol.
*/
@property (readonly) NSDictionary* serverDescription;

/*!
YES if clients are currently attached, NO otherwise. If you generate frames frequently (for instance on a display-link timer), you may choose to test this and only call publishFrameTexture:textureTarget:imageRegion:textureDimensions:flipped: when clients are attached.
*/
@property (readonly) BOOL hasClients;

/** @} */
/** @name Publishing Frames */
/** @{ */

/*!
 Returns a new client instance for the described server. You should check the isValid property after initialization to ensure a connection was made to the server.
 @param textureToPublish The MTLTexture you wish to publish on the server.
 @param commandBuffer Your commandBuffer on which Syphon will write its internal metal commands - You are responsible for comitting your commandBuffer yourself
 @param region The sub-region of the texture to publish.
 @param isFlipped Is the texture flipped?
*/
- (void)publishFrameTexture:(id<MTLTexture>)textureToPublish onCommandBuffer:(id<MTLCommandBuffer>)commandBuffer imageRegion:(NSRect)region flipped:(BOOL)isFlipped;

/*!
 Returns a MTLTexture representing the current output from the server, valid on the server's MTLDevice. Call this method every time you wish to access the current server frame. This object has a limited useful lifetime, and may have GPU resources associated with it: you should release it as soon as you are finished drawing with it.
  
 @returns A MTLTexture representing the current output from the server. YOU ARE RESPONSIBLE FOR RELEASING THIS OBJECT when you are finished with it.
 */
- (nullable id<MTLTexture>)newFrameImage;

/*!
 Stops the server instance. Use of this method is optional and releasing all references to the server has the same effect.
*/
- (void)stop;

/** @} */

@end

NS_ASSUME_NONNULL_END

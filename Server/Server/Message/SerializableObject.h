#ifndef SERIALIZABLE_OBJECT_H
#define SERIALIZABLE_OBJECT_H

#include "Utils.h"

const int SERIALIZE_ERROR = -1;

/**
 * This class defines an object that can be serialized/deserialized.
 */
class SerializableObject {
public:
    /**
     * Default constructor.
     */
    SerializableObject();

    /**
     * Destructor.
     */
    virtual ~SerializableObject();

    /**
     * This method returns the size of the object if it was serialized.
     *
     * @return The size of the serialized object.
     */
    virtual uint_t getSize() const = 0;

    /**
     * This method deserializes an object from a buffer.
     *
     * @param buffer The buffer containing a serialized object.
     * @param size   The size of the buffer.
     *
     * @return A pointer to a newly allocated object, or `nullptr` if the
     *         deserialization failed.
     */
    static SerializableObject* deserialize(const uint8_t* buffer, uint_t size);

    /**
     * This method serializes the object into a buffer.
     *
     * @param buffer The buffer used for the serialization.
     * @param size   The size of the buffer.
     *
     * @return The size of the serialized object, or SERIALIZE_ERROR if the
     *         buffer was too small to contains the serialized object.
     */
    virtual int serialize(uint8_t* buffer, uint_t size) const = 0;
};

#endif

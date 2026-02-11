def can_build(env, platform):
    """
    MQTT Protocol module can build on all platforms.
    """
    return True


def configure(env):
    """
    Configure build environment for MQTT Protocol module.
    """
    pass


def get_doc_classes():
    """
    Return list of classes to include in documentation.
    """
    return [
        "MQTTClient",
        "MQTTMessage",
        "ProtocolManager",
    ]


def get_doc_path():
    """
    Return path to documentation.
    """
    return "doc_classes"

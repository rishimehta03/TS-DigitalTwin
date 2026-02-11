def can_build(env, platform):
    """
    Digital Twin module can build on all platforms.
    """
    return True


def configure(env):
    """
    Configure build environment for Digital Twin module.
    """
    pass


def get_doc_classes():
    """
    Return list of classes to include in documentation.
    """
    return [
        "DigitalTwin",
        "TwinRegistry",
        "TwinState",
        "TwinEvents",
    ]


def get_doc_path():
    """
    Return path to documentation.
    """
    return "doc_classes"

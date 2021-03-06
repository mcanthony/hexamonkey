#include "core/objecttype.h"
#include "core/objecttypetemplate.h"
#include "core/parser.h"
#include "core/log/logmanager.h"
#include "core/variable/variablecollector.h"

#include "core/variable/typescope.h"

#define A_COUNT 0
#define A_ELEMENT_TYPE 1
#define A_ELEMENT_COUNT 2
#define A_NAME 3

const std::unordered_map<std::string, int> reserved = {
    {"@count", A_COUNT},
    {"@elementType", A_ELEMENT_TYPE},
    {"@elementCount", A_ELEMENT_COUNT},
    {"@name", A_NAME}
};

class TypeNameVariableImplementation : public VariableImplementation
{
public:
    TypeNameVariableImplementation(VariableCollector& collector, ObjectType& type)
        : VariableImplementation(collector),
          _type(type) {

    }

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.type() == Variant::stringType) {
            _type.setName(value.toString());
        } else {
            Log::warning("Trying to set an invalid value ", value, " as name for type ", _type);
        }
    }

    virtual Variant doGetValue() override {
        return Variant(_type.name());
    }
private:
    ObjectType& _type;
};

class ConstTypeNameVariableImplementation : public VariableImplementation
{
public:
    ConstTypeNameVariableImplementation(VariableCollector& collector, const ObjectType& type)
        : VariableImplementation(collector),
          _type(type)
    {

    }

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.name());
    }
private:
    const ObjectType& _type;
};

class TypeElementTypeVariableImplementation : public VariableImplementation
{
public:
    TypeElementTypeVariableImplementation(VariableCollector& collector, ObjectType& type)
        : VariableImplementation(collector),
          _type(type)
    {

    }

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.type() == Variant::objectType) {
            _type.setElementType(value.toObjectType());
        } else {
            Log::warning("Trying to set an invalid value ", value, " as element type for type ", _type);
        }
    }

    virtual Variant doGetValue() override {
        return Variant(_type.elementType());
    }
private:
    ObjectType& _type;
};

class ConstTypeElementTypeVariableImplementation : public VariableImplementation
{
public:
    ConstTypeElementTypeVariableImplementation(VariableCollector& collector, const ObjectType& type)
        : VariableImplementation(collector),
          _type(type)
    {

    }

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.elementType());
    }
private:
    const ObjectType& _type;
};

class TypeElementCountVariableImplementation : public VariableImplementation
{
public:
    TypeElementCountVariableImplementation(VariableCollector& collector, ObjectType& type)
        : VariableImplementation(collector),
          _type(type)
    {

    }

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.hasNumericalType()) {
            _type.setElementCount(value.toInteger());
        } else {
            Log::warning("Trying to set an invalid value ", value, " as element type for type ", _type);
        }
    }

    virtual Variant doGetValue() override {
        return Variant(_type.elementCount());
    }
private:
    ObjectType& _type;
};

class ConstTypeElementCountVariableImplementation : public VariableImplementation
{
public:
    ConstTypeElementCountVariableImplementation(VariableCollector& collector, const ObjectType& type)
        : VariableImplementation(collector),
          _type(type) {}

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.elementCount());
    }
private:
    const ObjectType& _type;
};

AbstractTypeScope::AbstractTypeScope(VariableCollector &collector)
    : VariableImplementation(collector)
{
}

Variant AbstractTypeScope::doGetValue()
{
    return constType();
}

Variable AbstractTypeScope::doGetField(const Variant &key, bool modifiable, bool createIfNeeded)
{
    ObjectType* mType = modifiable ? modifiableType() : nullptr;
    const ObjectType& cType = constType();

    int64_t parameterIndex = -1LL;
    const size_t numberOfParameters = cType.numberOfParameters();

    if (key.hasNumericalType()) {
        parameterIndex = key.toInteger();
        if (parameterIndex < 0 || parameterIndex >= numberOfParameters) {
            if (createIfNeeded) {
                Log::error("Trying to assign a value to an out of bounds parameter");
            }

            return Variable();
        }
    } else if (key.type() == Variant::stringType) {
        const std::string str = key.toString();



        if (!str.empty() && str[0]=='@') {
            auto it = reserved.find(str);
            if(it == reserved.end()) {
                Log::error("Unknown reserved field", str," for type ", cType);

                return Variable();
            }

            const int tag = it->second;

            switch(tag) {
            case A_COUNT:
                return collector().copy(numberOfParameters, false);

            case A_ELEMENT_TYPE:
                if (mType != nullptr) {
                    return Variable(new TypeElementTypeVariableImplementation(collector(), *mType), true);
                } else {
                    return Variable(new ConstTypeElementTypeVariableImplementation(collector(), cType), false);
                }

            case A_ELEMENT_COUNT:
                if (mType != nullptr) {
                    return Variable(new TypeElementCountVariableImplementation(collector(), *mType), true);
                } else {
                    return Variable(new ConstTypeElementCountVariableImplementation(collector(), cType), false);
                }

            case A_NAME:
                if (mType != nullptr) {
                    return Variable(new TypeNameVariableImplementation(collector(), *mType), true);
                } else {
                    return Variable(new ConstTypeNameVariableImplementation(collector(), cType), false);
                }
            }

            return Variable();
        } else {
            parameterIndex = cType.typeTemplate().parameterNumber(str);

            if (parameterIndex == -1) {
                if (createIfNeeded) {
                    Log::error("Trying to modify an unknown named parameter ", str, " for type ", cType);
                }
                return Variable();
            }

        }
    } else {
        if (createIfNeeded) {
            Log::error("Trying to modify an invalid parameter ", key, " for type ", cType);
        }
        return Variable();
    }

    if (parameterIndex != -1) {
        if(mType) {
            return collector().ref(mType->parameterValue(parameterIndex));
        } else {
            return collector().copy(cType.parameterValue(parameterIndex));
        }
    } else {
        return Variable();
    }
}

TypeScope::TypeScope(VariableCollector& collector, ObjectType &type, bool modifiable)
    : AbstractTypeScope(collector),
      _type(modifiable? &type : nullptr),
      _constType(type)
{
}

TypeScope::TypeScope(VariableCollector& collector, const ObjectType &type)
    : AbstractTypeScope(collector),
      _type(nullptr),
      _constType(type)
{
}

ObjectType *TypeScope::modifiableType()
{
    return _type;
}

const ObjectType &TypeScope::constType()
{
    return _constType;
}

ParserTypeScope::ParserTypeScope(Parser& parser)
    : AbstractTypeScope(parser.object().collector()),
      _object(parser.object()),
      _sharedType(parser.sharedType())
{
}

ObjectType *ParserTypeScope::modifiableType()
{
    if (_sharedType->first) {
        Log::error("Cannot modify parser type's once it has been parsed");
        return nullptr;
    } else {
        return &_object.type();
    }
}

const ObjectType &ParserTypeScope::constType()
{
    if (_sharedType->first) {
        return _sharedType->second;
    } else {
        return _object.type();
    }
}

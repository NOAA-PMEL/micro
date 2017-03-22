from FLEX_Simulator import FLEX as FLEX
import time

NumCycleTest = 1

def test_init_class():
    f = FLEX()

    assert(f.heading == [])
    assert(f.pitch == [])
    assert(f.roll == [])
    assert(f.power == False)
    assert(f.FilterSet == False)
    assert(f.MtgSet == False)


def test_power_on():
    f = FLEX()
    f.SP300xD_2_PowerOn()
    assert(f.power == True)

def test_power_off():
    f = FLEX()
    f.SP300xD_2_PowerOff()
    assert(f.power == False)


def test_init_sparton():
    f = FLEX()

    f.Init_Sp300xD_2()
    assert(f.FilterSet == True)
    assert(f.MtgSet == True)
    f.SP300xD_2_PowerOff()
    assert(f.power == False)
    


def test_retreive_direction():
    f = FLEX()

    f.Init_Sp300xD_2()
    assert(f.FilterSet == True)
    assert(f.MtgSet == True)

    time.sleep(1)
    #.SP300xD_2_Direction_Command()
    f.Get_SP300XD_2_Data()

    assert(f.heading >= 0.0)
    assert(f.heading <= 360.0)
    assert(f.pitch >= -90.0)
    assert(f.pitch <= 90.0)
    assert(f.roll >= -180.0)
    assert(f.roll <= 180.0)

    f.SP300xD_2_PowerOff()
    assert(f.power == False)

def test_continuous_data():
    f = FLEX()
    f.Init_Sp300xD_2()
    assert(f.FilterSet == True)
    assert(f.MtgSet == True)

    time.sleep(0.1)
    f.Get_SP300XD_2_Data()

    for i in range(0,NumCycleTest):
        f.Get_SP300XD_2_Data()

        assert(f.heading >= 0.0)
        assert(f.heading <= 360.0)
        assert(f.pitch >= -90.0)
        assert(f.pitch <= 90.0)
        assert(f.roll >= -180.0)
        assert(f.roll <= 180.0)

    f.SP300xD_2_PowerOff()
    assert(f.power == False)


def test_invalid_data():
    f = FLEX()
    f.Init_Sp300xD_2()
    assert(f.FilterSet == True)
    assert(f.MtgSet == True)

    time.sleep(0.1)

    f.Test1_SP300xD_2_InvalidValues()
    time.sleep(0.1)

    #Check to see if it recovered
    f.Get_SP300XD_2_Data()
    assert(f.heading >= 0.0)
    assert(f.heading <= 360.0)
    assert(f.pitch >= -90.0)
    assert(f.pitch <= 90.0)
    assert(f.roll >= -180.0)
    assert(f.roll <= 180.0)
